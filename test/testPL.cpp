/*
 * File:   testPL.cpp
 * Author: amine
 *
 * Created on 24 septembre 2017, 14:32
 */

#include <cstdlib>
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Basket.hpp"
#include <ctime>
#include <iostream>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

using namespace std;

/*
 *
 */
int main(int argc, char** argv) {

    float temps;
    clock_t t1, t2;
    t1 = clock();
    PnlMat *data = pnl_mat_create(1000, 40);

    PnlVect *sigma = pnl_vect_create_from_scalar(40, 0.2);
    PnlVect *spot = pnl_vect_create_from_scalar(40, 100.0);
    PnlVect *weights = pnl_vect_create_from_scalar(40, 0.025);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    BlackScholesModel *bsm = new BlackScholesModel(40, 0.04879, 0.0, sigma, spot);
    bsm->trend_ = pnl_vect_create_from_scalar(40, 0.04);
    bsm->simul_market(data, 1000, 1.0, rng);

    Basket *basket = new Basket(3.0, 1, 40, 100.0, weights);

    MonteCarlo *monteCarlo = new MonteCarlo(bsm, basket, rng, 0.1, 100);

    double pL = monteCarlo->pAndL(data);
    t2 = clock();
    temps = (float) (t2 - t1) / CLOCKS_PER_SEC;
    cout << temps << endl;
    cout << pL;
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_vect_free(&weights);
    pnl_rng_free(&rng);
    pnl_mat_free(&data);
    delete basket;
    delete bsm;
    delete monteCarlo;



    return 0;
}
