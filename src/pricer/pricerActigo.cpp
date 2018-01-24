#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Actigo.hpp"
#include "Option.hpp"
#include <string>
#include "time_utils.hpp"
#include <iostream>

using namespace std;


int main(int argc, char **argv){
  //Actigo's maturity
  double maturity = 8.0;
  //Actigo option first date
  char* startDate = "2015-10-12";
  //Actigo option end Date
  char* endDate = "2023-11-04";
  //Today's date
  time_t todayEpoch = currentEpoch();
  char* currentDate = epochToDate(todayEpoch);
  //Recuperate data from startDate to currentDate
  Data *data = new Data(startDate, currentDate)
  //create Actigo option with 2016 = 252*8 nbtimeSteps
  Actigo actigo* = new Actigo(8., 2016, GET(data->euroStoxSpots, 0),
                              GET(data->spUsdSpots, 0), GET(data->spAudSpots, 0));
  //Calibrate volatilities and correlations for extracted data
  Calibration *calibration = new Calibration(data);
  //completeData from currentDate to endDate;
  int remainingDates = 2016 - data->euroStoxSpots->size; 
  data->completeData(remainingDates, actigo, currentSpots, volatilities, correlations);





  return 0;
}
