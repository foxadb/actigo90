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
  //Actigo's maturity
  double maturity = 8.0;
  //Actigo option first date
  //char* startDate = "2015-10-12";
  //Actigo option end Date
  //char* endDate = "2023-11-04";
  //Today's date
  time_t todayEpoch = currentEpoch();
  //string currentDate = epochToDate(todayEpoch);
  //Recuperate data from startDate to currentDate
  //Data *data = new Data("2015-10-12", "2017-12-30");
  //create Actigo option with 2016 = 252*8 nbtimeSteps
  Actigo *actigo = new Actigo(8., 2016, 5, 100., 100., 100.);
  PnlVect *sigma = pnl_vect_create_from_scalar(5, 0.5);
  PnlVect *spots = pnl_vect_create_from_scalar(5, 100.);
  BlackScholesModel *bsm = new BlackScholesModel(5, 0.04, 0.2, sigma,spots);
  //bsm->
  //Data *data = new Data()
  //Calibrate volatilities and correlations for extracted data
  //Calibration *calibration = new Calibration(data);
  //completeData from currentDate to endDate;
  //int remainingDates = 2016 - data->euroStoxSpots->size;
  //data->completeData(remainingDates, actigo, currentSpots, volatilities, correlations);





  return 0;
}
