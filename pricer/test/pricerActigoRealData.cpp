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
    Data *data = new Data("2009-01-07","2017-01-23");
    cout << "Historical matrix size: " << data->historicalDataMatrix->m << endl;

    //// Data calibration
    // Create Actigo Option
    double maturity = 8.0;
    double step = maturity / data->historicalDataMatrix->m;
    Calibration *calibration = new Calibration(data, step);
    int size = 5;
    PnlVect *initialSpots = pnl_vect_create(size);
    PnlVect *initialSpotsEuro = pnl_vect_create(size);
    PnlVect *todaySpots = pnl_vect_create(size);
    data->getInitialSpots(initialSpots);
    data->getInitialSpotsEuro(initialSpotsEuro);
    data->getTodaySpots(todaySpots);
    Actigo *actigo = new Actigo(maturity, 16, size, GET(initialSpots,0), GET(initialSpots, 1), GET(initialSpots, 2));
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    // Create the BlackScholesModel
    double rEur = 0.04;
    BlackScholesModel *bsm = new BlackScholesModel(size, rEur, calibration->getCorrelationsMatrix(), calibration->getVolatilities(), initialSpotsEuro);

    // Create Monte-Carlo Simulation
    int nbSamples = atoi(argv[1]);
    double fdStep = 0.01;

    cout << "Samples number: " << nbSamples << endl;

    MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, fdStep, nbSamples);

    // Compute P&L
    clock_t start = clock();
    double pnl = mc->pAndL(data->historicalDataMatrixEuro);
    double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << "P&L: " << pnl * 100 << " %" << endl;
    cout << "P&L duration: " << duration << " seconds" << endl;

    return EXIT_SUCCESS;
}
