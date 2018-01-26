#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Actigo.hpp"
#include "Option.hpp"
#include "Data.hpp"
#include "Calibration.hpp"
#include <string>
#include "../quote/time_utils.hpp"
#include <iostream>

using namespace std;


int main(int argc, char **argv){
  //Recuperate data from actigo first day: "2015-10-12" to today
  Data *data = new Data("2015-10-12","2018-01-24");
  cout << "size" << data->historicalDataMatrix->m << endl;
  //Calibrate data
  // create Actigo Option
  double maturity = 8.;
  int nbTimeSteps = 2016;
  double step = maturity / nbTimeSteps;
  Calibration *calibration = new Calibration(data, step);
  int size = 5;
  PnlVect *initialSpots = pnl_vect_create(size);
  PnlVect *todaySpots = pnl_vect_create(size);
  data->getInitialSpots(initialSpots);
  data->getTodaySpots(todaySpots);
  Actigo *actigo = new Actigo(maturity, nbTimeSteps, size, GET(initialSpots,0), GET(initialSpots, 1), GET(initialSpots, 2));
  //complete data from today to actigo end date
  int remainingDates = nbTimeSteps - data->euroStoxSpots->size;
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  data->completeData(remainingDates, actigo, todaySpots, calibration->getVolatilities(),
                    calibration->getCorrelationsMatrix(), calibration->getTrends(), rng);
  //create the BlackScholesModel
  double rEur = 0.04;
  BlackScholesModel *bsm = new BlackScholesModel(size, rEur, calibration->getCorrelationsMatrix(), calibration->getVolatilities(), initialSpots);
  //create monteCarlo Simulation
  int nbSamples = 50000;
  double fdStep = 0.01;

  MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, fdStep, nbSamples);
  double pl = mc->pAndL(data->completeDataMatrix);
  cout << " the P&L is " << pl << endl;
  return 0;
}
