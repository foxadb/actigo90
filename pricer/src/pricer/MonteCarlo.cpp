#include <cstdio>
#include <cmath>
#include <iostream>

#include "MonteCarlo.hpp"

using namespace std;

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples) {
    mod_ = mod;
    opt_ = opt;
    rng_ = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_clone(rng_, rng);
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    path = pnl_mat_create(opt_->nbTimeSteps_ + 1, opt_->size_);
    shiftPath = pnl_mat_create(opt_->nbTimeSteps_ + 1, opt_->size_);
}

void MonteCarlo::price(double &prix, double &ic) {
    double variance = 0;
    double payOff = 0;
    pnl_mat_set_row(path, mod_->spot_, 0);
    int nb = opt_->nbTimeSteps_;
    double mat = opt_->T_;
    for (int i = 0; i < nbSamples_; i++) {
        mod_->asset(path, mat, nb,rng_);
        payOff = opt_->payoff(path);
        prix += payOff;
        variance += pow(payOff, 2);
    }

    double esperance = prix / nbSamples_;
    prix = exp(-mod_->r_ * opt_->T_) * prix / nbSamples_;
    variance = exp(-2.0 * mod_->r_ * opt_->T_) * (variance / nbSamples_ - pow(esperance, 2.0));
    ic = sqrt(variance / nbSamples_);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta) {
    double payOffDifference;

    for (int j = 0; j < nbSamples_; j++) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        payOffDifference = 0.0;
        for (int d = 0; d < opt_->size_; d++) {
            mod_->shiftAsset(shiftPath, path, d, fdStep_, t, opt_->T_ / opt_->nbTimeSteps_);
            payOffDifference = opt_->payoff(shiftPath);
            mod_->shiftAsset(shiftPath, path, d, -fdStep_, t, opt_->T_ / opt_->nbTimeSteps_);
            payOffDifference -= opt_->payoff(shiftPath);
            pnl_vect_set(delta, d, pnl_vect_get(delta, d) + payOffDifference);
        }
    }

    PnlVect *scalar = pnl_vect_create_from_scalar(opt_->size_, exp(mod_->r_ * (t - opt_->T_))
                                                  / (2.0 * fdStep_ * nbSamples_));

    PnlVect *lastPrices = pnl_vect_create(opt_->size_);
    double x;
    int vect = past->m - 1;
    for (int i = 1; i < past->m; i++) {
        x = pnl_mat_get(past, i, 0);
        if (x == 0) {
            vect = i - 1;
            break;
        }
    }

    pnl_mat_get_row(lastPrices, past, vect);
    pnl_vect_div_vect_term(scalar, lastPrices);
    pnl_vect_mult_vect_term(delta, scalar);
    pnl_vect_free(&scalar);
    pnl_vect_free(&lastPrices);
}

void MonteCarlo::price(const PnlMat* past, double t, double& prix, double& ic) {
    double variance = 0;

    for (int i = 0; i < nbSamples_; ++i) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        double x = opt_->payoff(path);
        prix += x;
        variance += pow(x, 2);
    }

    double esperance = prix / nbSamples_;
    prix = exp(-mod_->r_ * (opt_->T_ - t)) * prix / nbSamples_;
    variance = exp(-2.0 * mod_->r_ * (opt_->T_ - t)) * (variance / nbSamples_ - pow(esperance, 2.0));
    ic = sqrt(variance / nbSamples_);
}

MonteCarlo::~MonteCarlo() {
    pnl_mat_free(&path);
    pnl_rng_free(&rng_);
    pnl_mat_free(&shiftPath);
}

double MonteCarlo::pAndL(PnlMat *data) {
    double prix, ic;

    PnlMat *past = pnl_mat_create_from_scalar(opt_->nbTimeSteps_ + 1, data->n, 0);
    PnlVect *delta = pnl_vect_create_from_scalar(data->n, 0);
    PnlVect *pastDelta = pnl_vect_create(data->n);
    PnlVect *spot = pnl_vect_create(data->n);
    pnl_vect_clone(spot, mod_->spot_);
    price(prix, ic);
    cout << "price at 0: " << prix << endl;
    pnl_mat_set_row(past, spot, 0);
    double pas = opt_->T_ / data->m;
    double temps = 0.0;
    this->delta(past, 0.0, delta);
    cout << endl << "Deltas at 0:" << endl;
    pnl_vect_print(delta);
    double v = prix - pnl_vect_scalar_prod(delta, spot);
    cout << endl << "Portfolio value at 0: " << v << endl << endl;
    double param = mod_->r_ * opt_->T_ / data->m;

    cout << "P&L computation..." << endl;
    for (int i = 1; i < data->m; i++) {
        //cout << i << endl;
        updatePast(past, data, i);
        pnl_vect_clone(pastDelta, delta);
        temps = temps + pas;
        pnl_vect_set_all(delta, 0.0);
        this->delta(past, temps, delta);
        pnl_mat_get_row(spot, data, i);
        pnl_vect_minus_vect(pastDelta, delta);
        v = v * exp(param) + pnl_vect_scalar_prod(pastDelta, spot);
    }
    double produit = pnl_vect_scalar_prod(delta, spot);
    pnl_vect_free(&delta);
    pnl_vect_free(&pastDelta);
    pnl_vect_free(&spot);
    pnl_mat_free(&past);
    PnlMat *dataAtNbTimeSteps = pnl_mat_create(opt_->nbTimeSteps_ + 1, opt_->size_);
    double pathStep = opt_->T_ / opt_->nbTimeSteps_;
    int newStep = pathStep / pas;
    PnlVect* vect = pnl_vect_create(opt_->size_);
    int count = 0;

    for (int i = 0; i < data->m; i += newStep) {
        pnl_mat_get_row(vect, data, i);
        pnl_mat_set_row(dataAtNbTimeSteps, vect, count);
        ++count;
    }
    PnlVect* vectfinal = pnl_vect_create_from_scalar(5,0.);
    pnl_mat_get_row(vectfinal, data, data->m-1);
    pnl_mat_set_row(dataAtNbTimeSteps, vectfinal, dataAtNbTimeSteps->m-1);
    return v + produit - opt_->payoff(dataAtNbTimeSteps);
}

void MonteCarlo::updatePast(PnlMat *past, PnlMat *data, int i) {
    int step = data->m / opt_->nbTimeSteps_;
    PnlVect *tmpVect = pnl_vect_create(data->n);
    pnl_mat_get_row(tmpVect, data, i);
    pnl_mat_set_row(past, tmpVect, (i - 1) / step + 1);
    pnl_vect_free(&tmpVect);
}

double MonteCarlo::pAndL(PnlMat *delta, PnlMat* data, PnlMat* semestrialData, double priceAtZero){
  PnlVect *currentDelta = pnl_vect_create_from_scalar(5, 0.);
  PnlVect *pastDelta = pnl_vect_create_from_scalar(5, 0.);
  PnlVect *currentSpots = pnl_vect_create_from_scalar(5, 0.);
  pnl_mat_get_row(currentDelta, delta, 0);
  pnl_mat_get_row(currentSpots, data, 0);
  double actuParam = exp(mod_->r_*opt_->T_ / data->m);
  double v = priceAtZero - pnl_vect_scalar_prod(currentDelta, currentSpots);
  for ( int i = 1; i < data->m ; i++){
    pnl_vect_clone(pastDelta, currentDelta);
    pnl_mat_get_row(currentDelta, delta, i);
    pnl_mat_get_row(currentSpots, data, i);
    pnl_vect_minus_vect(pastDelta, currentDelta);
    v = v * actuParam + pnl_vect_scalar_prod(pastDelta, currentSpots);
  }
  double lastDeltaPrice = pnl_vect_scalar_prod(currentDelta, currentSpots);
  v = v + lastDeltaPrice - opt_->payoff(semestrialData);
  //substract payoff
  pnl_vect_free(&currentDelta);
  pnl_vect_free(&currentSpots);
  pnl_vect_free(&pastDelta);
  return v;
}

void MonteCarlo::rebalanceAtSpecificDate(PnlMat *past, double date, PnlVect *delta, double &price){
  double ic;
  this->delta(past, date, delta);
  this->price(past, date, price, ic);
}
