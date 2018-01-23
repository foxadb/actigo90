#include "MonteCarlo.hpp"
#include "Basket.hpp"
#include "BlackScholesModel.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    float temps;
    clock_t t1, t2;

    PnlVect *weights = pnl_vect_create_from_scalar(40, 0.025);
    PnlVect *sigma = pnl_vect_create_from_scalar(40, 0.2);
    PnlVect *spot = pnl_vect_create_from_scalar(40, 100.0);

    Basket *basket1 = new Basket(3.0, 1, 40, 100.0, weights);
    BlackScholesModel *bsm = new BlackScholesModel(40, 0.04879, 0.0, sigma, spot);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    double prix1, prix2;
    double ic1, ic2;
    Basket *basket2 = new Basket(4.0, 1, 40, 100.0, weights);
    MonteCarlo *mc = new MonteCarlo(bsm, basket1, rng, 0.1, 50000);
    MonteCarlo *mc2 = new MonteCarlo(bsm, basket2, rng, 0.1, 50000);
    PnlMat *past = pnl_mat_create_from_scalar(1, 40, 100.0);
    t1 = clock();
    mc->price(prix1, ic1);
    mc2->price(past, 1.0, prix2, ic2);
    t2 = clock();
    temps = (float) (t2 - t1) / CLOCKS_PER_SEC;
    cout << prix1 << endl;
    cout << prix2 << endl;
    cout << ic1 << endl;
    cout << ic2 << endl;
    cout << temps << endl;
    pnl_vect_free(&weights);
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_rng_free(&rng);
    pnl_mat_free(&past);
    delete bsm;
    delete basket1;
    delete basket2;
    delete mc;
    delete mc2;

    return 0;
}
