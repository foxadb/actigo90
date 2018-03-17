#include <iostream>
#include <string>
#include <ctime>

#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Actigo.hpp"
#include "Option.hpp"
#include "Data.hpp"
#include "Calibration.hpp"

using namespace std;

int main(int argc, char **argv) {
    //Recuperate data from actigo first day: "2015-10-12" to today

    //// Data calibration
    // Create Actigo Option
    double maturity = 8;
    int nbTimeSteps = 2016;
    double step = maturity / nbTimeSteps;
    int size = 5;
    // Create the BlackScholesModel
    double rEur = 0.04;
    double rho = 0.;
    PnlVect *vols = pnl_vect_create_from_scalar(size, 0.04);
    PnlVect *initialSpots = pnl_vect_create_from_scalar(size, 3000.0);
    LET(initialSpots, 1) = 2500;
    LET(initialSpots, 2) = 1600;
    LET(initialSpots, 3) = 0.7;
    LET(initialSpots, 4) = 0.75;
    BlackScholesModel *bsm = new BlackScholesModel(size, rEur, rho, vols, initialSpots);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    PnlVect* trend = pnl_vect_create_from_scalar(5, rEur);
    PnlMat* data = pnl_mat_create_from_scalar(2016, 5, 0.);
    bsm->trend_ = trend;
    bsm->simul_market(data, nbTimeSteps, maturity, rng);
    PnlVect* spUsdspots = pnl_vect_create_from_scalar(nbTimeSteps, 0.);
    PnlVect* spAudspots = pnl_vect_create_from_scalar(nbTimeSteps, 0.);
    PnlVect* eurUsd = pnl_vect_create_from_scalar(nbTimeSteps, 0.);
    PnlVect* eurAud = pnl_vect_create_from_scalar(nbTimeSteps, 0.);
    pnl_mat_get_col(spUsdspots, data, 1);
    pnl_mat_get_col(spAudspots, data, 2);
    pnl_mat_get_col(eurUsd, data, 3);
    pnl_mat_get_col(eurAud, data, 4);
    pnl_vect_mult_vect_term(spUsdspots, eurUsd);
    pnl_vect_mult_vect_term(spAudspots, eurAud);
    pnl_mat_set_col(data, spUsdspots, 1);
    pnl_mat_set_col(data, spAudspots, 2);
    Actigo *actigo = new Actigo(maturity, 16, size, GET(initialSpots,0), GET(initialSpots, 1), GET(initialSpots, 2));
    // Complete data from today to actigo end date
    PnlVect* initialSpotsEuro = pnl_vect_create_from_scalar(size, 0.);
    pnl_mat_get_row(initialSpotsEuro, data, 0);
    LET(vols, 1) = sqrt(2*0.04*0.04);
    LET(vols, 2) = sqrt(2*0.04*0.04);
    PnlMat* correlations = pnl_mat_create_from_scalar(5,5,rho);
    for (int d = 0; d < 5; d++) {
        pnl_mat_set_diag(correlations, 1.0, d);
    }
    MLET(correlations, 1, 3) = 0.04;
    MLET(correlations, 3, 1) = 0.04;
    MLET(correlations, 2, 4) = 0.04;
    MLET(correlations, 4, 2) = 0.04;
    BlackScholesModel *bsm2 = new BlackScholesModel(size, rEur, correlations, vols, initialSpotsEuro);

    // Create Monte-Carlo Simulation
    int nbSamples = atoi(argv[1]);
    double fdStep = 0.01;

    cout << "Samples number: " << nbSamples << endl;

    MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, fdStep, nbSamples);

    // Compute P&L
    clock_t start = clock();
    double pnl = mc->pAndL(data);
    double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << "P&L: " << pnl * 100 << " %" << endl;
    cout << "P&L duration: " << duration << " seconds" << endl;

    return EXIT_SUCCESS;
}
