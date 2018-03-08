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
    Data *data = new Data("2011-04-10","2012-04-14");
    cout << "Historical matrix size: " << data->historicalDataMatrix->m << endl;

    //// Data calibration
    // Create Actigo Option
    int rebalancingFrequency = 2;
    int totalRebalancingDates = data->historicalDataMatrix->m / rebalancingFrequency;
    cout << "rebalancingDates " << totalRebalancingDates << endl;
    PnlMat *path = pnl_mat_create_from_scalar(totalRebalancingDates, 5, 0);
    cout << "lines" << path->m << endl;
    data->getDataAtRebalancingDates(path, rebalancingFrequency);
    double maturity = 8;
    double step = rebalancingFrequency / 365.;
    Calibration *calibration = new Calibration(data, step);
    int size = 5;
    PnlVect *initialSpots = pnl_vect_create(size);
    data->getInitialSpots(initialSpots);
    PnlVect *initialSpotsEuro = pnl_vect_create(size);
    cout << "created"<< endl;
    data->getInitialSpots(initialSpotsEuro);

    Actigo *actigo = new Actigo(maturity, 16, size, GET(initialSpots,0), GET(initialSpots, 1), GET(initialSpots, 2));
    // Complete data from today to actigo end date
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
    double pnl = mc->pAndL(path);
    double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << "P&L: " << pnl * 100 << " %" << endl;
    cout << "P&L duration: " << duration << " seconds" << endl;
    //pnl_mat_free(&path);*/

    return EXIT_SUCCESS;
}
