/* 
 * File:   testDelta.cpp
 * Author: bernoush
 *
 * Created on September 22, 2017, 9:19 AM
 */

#include <cstdlib>
#include "MonteCarlo.hpp"
#include "Basket.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>
#include <stdio.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    PnlVect *weights = pnl_vect_create_from_scalar(40, 0.025);
    PnlVect *sigma = pnl_vect_create_from_scalar(40, 0.2);
    PnlVect *spot = pnl_vect_create_from_scalar(40, 100.0);

    Basket *basket = new Basket(3.0, 1, 40, 100.0, weights);
    BlackScholesModel *bsm = new BlackScholesModel(40, 0.04879, 0.0, sigma, spot);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    double prix;
    double ic;
    MonteCarlo *mc = new MonteCarlo(bsm, basket, rng, 0.1, 50000);
    PnlVect *delta = pnl_vect_create_from_scalar(40, 0.0);
    PnlMat *past = pnl_mat_create_from_scalar(1, 40, 100.0);
    mc->delta(past, 1.0, delta);
    pnl_vect_print(delta);
    pnl_vect_free(&weights);
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_rng_free(&rng);
    pnl_vect_free(&delta);
    pnl_mat_free(&past);
    delete basket;
    delete bsm;
    delete mc;
    
    
    
    return 0;
}

