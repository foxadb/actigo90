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
    double rEur = 0.0075;
    double rUsd = 0.028;
    double rAud = 0.026;
    Data *data = new Data("2015-10-12","2018-01-24", rEur, rUsd, rAud);
    cout << "Historical matrix size: " << data->historicalDataMatrix->m << endl;

    //// Data calibration
    // Create Actigo Option
    double maturity = 8;
    int nbTimeSteps = 2016;
    double step = maturity / nbTimeSteps;
    Calibration *calibration = new Calibration(data, step);
    int size = 5;
    PnlVect *initialSpots = pnl_vect_create(size);
    PnlVect *initialSpotsEuro = pnl_vect_create(size);
    PnlVect *todaySpots = pnl_vect_create(size);
    data->getInitialSpots(initialSpots);
    data->getInitialSpotsEuro(initialSpotsEuro);
    data->getTodaySpots(todaySpots);
    Actigo *actigo = new Actigo(maturity, 16, size, GET(initialSpots,0), GET(initialSpots, 1), GET(initialSpots, 2),
                                rEur, rUsd, rAud);
    // Complete data from today to actigo end date
    int remainingDates = nbTimeSteps - data->euroStoxSpots->size;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    data->completeData(remainingDates, actigo, todaySpots, calibration->getVolatilities(),
                       calibration->getCorrelationsMatrix(), calibration->getTrends(), rng);

    // Create the BlackScholesModel
    BlackScholesModel *bsm = new BlackScholesModel(size, rEur, calibration->getCorrelationsMatrix(), calibration->getVolatilities(), initialSpotsEuro);

    // Create Monte-Carlo Simulation
    int nbSamples = atoi(argv[1]);
    double fdStep = 0.01;

    cout << "Samples number: " << nbSamples << endl;

    MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, fdStep, nbSamples);

    // Compute P&L
    clock_t start = clock();
    double pnl = mc->pAndL(data->completeDataMatrix);
    double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << "P&L: " << pnl * 100 << " %" << endl;
    cout << "P&L duration: " << duration << " seconds" << endl;

    return EXIT_SUCCESS;
}
