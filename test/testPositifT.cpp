/* 
 * File:   testPositifT.cpp
 * Author: elfilalz
 *
 * Created on September 21, 2017, 10:08 AM
 */

#include <cstdlib>

#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Basket.hpp"
#include <ctime>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    clock_t t1 = clock();
    PnlMat *past = pnl_mat_create_from_scalar(1,40,100.0);
    PnlVect *sigma = pnl_vect_create_from_scalar(40, 0.2);
    PnlVect *spot = pnl_vect_create_from_scalar(40, 100.0);
    PnlVect *weights = pnl_vect_create_from_scalar(40, 0.025);
    BlackScholesModel *bsm = new BlackScholesModel(40, 0.04879, 0.2, sigma, spot);
    Basket *basket = new Basket(4.0, 1, 40, 100.0, weights);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    MonteCarlo *monteCarlo = new MonteCarlo(bsm, basket, rng, 0.1, 50000);
    double prix;
    double ic;
    monteCarlo->price(past, 1.0, prix, ic);
    pnl_mat_free(&past);
    cout << prix << endl;
    cout << ic << endl;
    clock_t t2 = clock();
    cout << (t2 - t1) / (double) CLOCKS_PER_SEC << endl;
    pnl_vect_free(&sigma);
    pnl_mat_free(&past);
    pnl_vect_free(&spot);
    pnl_vect_free(&weights);
    pnl_rng_free(&rng);
    delete bsm;
    delete basket;
    delete monteCarlo;
    return 0;
}

