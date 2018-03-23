#include "BlackScholesModel.hpp"
#include <cmath>
#include <iostream>

using namespace std;

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot) {
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = pnl_vect_new();
    pnl_vect_clone(sigma_, sigma);
    spot_ = pnl_vect_new();
    pnl_vect_clone(spot_, spot);
    gamma = pnl_mat_create_from_scalar(size_, size_, rho_);
    for (int d = 0; d < size_; d++) {
        pnl_mat_set_diag(gamma, 1.0, d);
    }
    sigmaCarre = pnl_vect_create(size_);
    pnl_vect_clone(sigmaCarre, sigma_);
    pnl_vect_mult_vect_term(sigmaCarre, sigmaCarre);
    pnl_vect_mult_scalar(sigmaCarre, -0.5);
    pnl_vect_plus_scalar(sigmaCarre, r_);
    int positivityGamma = pnl_mat_chol(gamma);
    g = pnl_vect_create(size_);
    newSpot = pnl_vect_create(size_);
    produit = pnl_vect_create(size_);
    coeff = pnl_vect_create(size_);
    pastVect = pnl_vect_create(size_);
    ptr = exp;
}

BlackScholesModel::BlackScholesModel(int size, double r, PnlMat *correlations, PnlVect *sigma, PnlVect *spot){
    size_ = size;
    r_ = r;
    sigma_ = pnl_vect_new();
    pnl_vect_clone(sigma_, sigma);
    spot_ = pnl_vect_new();
    pnl_vect_clone(spot_, spot);
    gamma = pnl_mat_new();
    pnl_mat_clone(gamma, correlations);
    sigmaCarre = pnl_vect_create(size_);
    pnl_vect_clone(sigmaCarre, sigma_);
    pnl_vect_mult_vect_term(sigmaCarre, sigmaCarre);
    pnl_vect_mult_scalar(sigmaCarre, -0.5);
    pnl_vect_plus_scalar(sigmaCarre, r_);
    int positivityGamma = pnl_mat_chol(gamma);
    g = pnl_vect_create(size_);
    newSpot = pnl_vect_create(size_);
    produit = pnl_vect_create(size_);
    coeff = pnl_vect_create(size_);
    pastVect = pnl_vect_create(size_);
    ptr = exp;
}


void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) {

    double step = T / nbTimeSteps;
    double sqrtStep = sqrt(step);
    pnl_mat_set_row(path, spot_, 0);

    pnl_vect_clone(coeff, sigmaCarre);
    pnl_vect_mult_scalar(coeff, step);

    for (int t = 1; t < nbTimeSteps + 1; t++) {

        pnl_vect_rng_normal(g, size_, rng);
        pnl_mat_get_row(newSpot, path, t - 1);
        pnl_mat_mult_vect_inplace(produit, gamma, g);
        pnl_vect_mult_scalar(produit, sqrtStep);
        pnl_vect_mult_vect_term(produit, sigma_);
        pnl_vect_plus_vect(produit, coeff);
        pnl_vect_map_inplace(produit, ptr);
        pnl_vect_mult_vect_term(produit, newSpot);
        pnl_mat_set_row(path, produit, t);

    }
}

void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {
    double step = T / nbTimeSteps;

    double size = past->m - 1;
    double x;
    int vect = past->m-1;
    for (int i = 1; i < past->m; i++) {
        x = pnl_mat_get(past, i, 0);
        if (x == 0) {
            vect = i - 1;
            break;
        }
    }
    pnl_vect_clone(coeff, sigmaCarre);
    pnl_vect_mult_scalar(coeff, step);

    double tSuiv = step * (vect);
    pnl_mat_set_subblock(path, past, 0, 0);
    pnl_mat_get_row(pastVect, past, vect);
    size = vect;
    double newStartingPoint = tSuiv - t;
    if (newStartingPoint < 0)
        newStartingPoint = 0;
    pnl_vect_rng_normal(g, size_, rng);
    pnl_mat_mult_vect_inplace(produit, gamma, g);
    pnl_vect_clone(coeff, sigmaCarre);
    pnl_vect_mult_scalar(coeff, newStartingPoint);
    pnl_vect_mult_vect_term(produit, sigma_);
    pnl_vect_mult_scalar(produit, sqrt(newStartingPoint));
    pnl_vect_plus_vect(produit, coeff);
    pnl_vect_map_inplace(produit, ptr);
    pnl_vect_mult_vect_term(produit, pastVect);
    pnl_mat_set_row(path, produit, size); // +1
    pnl_vect_clone(pastVect,produit);

    for (int i = size + 1; i < path->m; ++i) { // +2
        pnl_vect_rng_normal(g, size_, rng);
        pnl_mat_get_row(pastVect, path, i - 1);
        pnl_mat_mult_vect_inplace(produit, gamma, g);
        pnl_vect_clone(coeff, sigmaCarre);
        pnl_vect_mult_scalar(coeff,  step);
        pnl_vect_mult_vect_term(produit, sigma_);
        pnl_vect_mult_scalar(produit, sqrt(step));
        pnl_vect_plus_vect(produit, coeff);
        pnl_vect_map_inplace(produit, ptr);
        pnl_vect_mult_vect_term(produit, pastVect);
        pnl_mat_set_row(path, produit, i);
    }
}

void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) {
    pnl_mat_clone(shift_path, path);
    double verif = t / timestep;
    int intVerif = (int) verif;
    int indice = intVerif + 1;
    if (intVerif == verif) {
        indice--;
    }
    for (int i = indice; i < path->m; i++) {
        pnl_mat_set(shift_path, i, d, pnl_mat_get(path, i, d)* (1.0 + h));
    }

}

void BlackScholesModel::simul_market(PnlMat* path, int nbDates, double T, PnlRng* rng) {
    double step = T / nbDates;
    double sqrtStep = sqrt(step);
    pnl_mat_set_row(path, spot_, 0);

    pnl_vect_clone(coeff, sigmaCarre);
    pnl_vect_mult_scalar(coeff, -0.5);
    pnl_vect_plus_vect(coeff, trend_);
    pnl_vect_mult_scalar(coeff, step);

    for (int t = 1; t < nbDates; t++) {

        pnl_vect_rng_normal(g, size_, rng);
        pnl_mat_get_row(newSpot, path, t - 1);

        pnl_mat_mult_vect_inplace(produit, gamma, g);
        pnl_vect_mult_scalar(produit, sqrtStep);
        pnl_vect_mult_vect_term(produit, sigma_);
        pnl_vect_plus_vect(produit, coeff);

        double(*ptr)(double) = exp;
        pnl_vect_map(produit, produit, ptr);

        pnl_vect_mult_vect_term(produit, newSpot);
        pnl_mat_set_row(path, produit, t);
    }

}

BlackScholesModel::~BlackScholesModel() {
    pnl_vect_free(&sigma_);
    pnl_vect_free(&spot_);
    // TODO ce free provoque une segfault
    // pnl_vect_free(&trend_);
    pnl_vect_free(&sigmaCarre);
    pnl_mat_free(&gamma);
    pnl_vect_free(&g);
    pnl_vect_free(&newSpot);
    pnl_vect_free(&produit);
    pnl_vect_free(&coeff);
    pnl_vect_free(&pastVect);
}
