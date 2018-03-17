#include "DataBaseManager.hpp"
#include <vector>
#include "spot.hpp"
#include <iostream>
#include <iostream>
#include <string>
#include <ctime>
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Actigo.hpp"
#include "Option.hpp"
#include "Data.hpp"
#include "Calibration.hpp"
#include "time_utils.hpp"



void vectorToPnlVect(std::vector<Spot> spots, PnlVect* spotsVect){
  spotsVect = pnl_vect_create_from_scalar(spots.size(), 0.);
  int count = 0;
  for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it){
     LET(spotsVect, count) = (*it).getClose();
     count++;
  }
}


void getPastData(DataBaseManager *dbManager, PnlMat* past, std::vector<time_t> dates){
  int count = 0;
  PnlVect* euroStoxSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* spUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* spAudSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* eurUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* eurAudSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  for (std::vector<time_t>::iterator it = dates.begin(); it != dates.end(); ++it){
      LET(euroStoxSpots, count) = dbManager->getSpot(epochToDate((*it)), "^STOXX50E")->getClose();
      LET(spUsdSpots, count) = dbManager->getSpot(epochToDate((*it)), "^GSPC")->getClose();
      LET(spAudSpots, count) = dbManager->getSpot(epochToDate((*it)), "^AXJO")->getClose();
      LET(eurUsdSpots, count) = dbManager->getSpot(epochToDate((*it)), "EURUSD=X")->getClose();
      LET(eurAudSpots, count) = dbManager->getSpot(epochToDate((*it)), "EURAUD=X")->getClose();
      count++;
  }
  pnl_vect_mult_vect_term (spUsdSpots, eurUsdSpots);
  pnl_vect_mult_vect_term (spAudSpots, eurAudSpots);

  //constructing zero coupons
  //getZeroCoupon(eurUsd, rUsd, 8.0);
  //getZeroCoupon(eurAud, rAud, 8.0);

  pnl_mat_set_col(past, euroStoxSpots, 0);
  pnl_mat_set_col(past, spUsdSpots, 1);
  pnl_mat_set_col(past, spAudSpots, 2);
  pnl_mat_set_col(past, eurUsdSpots, 3);
  pnl_mat_set_col(past, eurAudSpots, 4);
}

std::vector<time_t> getRightDates(time_t today, std::vector<time_t> dates){
  std::vector<time_t> rightDates;
  for (std::vector<time_t>::iterator it = dates.begin(); it != dates.end(); ++it){
    if ( (*it) >= today)
      break;
    rightDates.push_back((*it));
  }
  rightDates.push_back(today);
  return rightDates;
}

int main(int argc, char **argv){
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  PnlVect* euroStoxSpots;
  PnlVect* spUsdSpots;
  PnlVect* spAudSpots;
  PnlVect* eurUsdSpots;
  PnlVect* eurAudSpots;

  /*std::vector<ctime> semesterDates {dateToEpoch("12-10-2015"), dateToEpoch("11-04-2016"), dateToEpoch("10-10-2016"), dateToEpoch("10-04-2017"),
    dateToEpoch("10-10-2017"), dateToEpoch("10-04-2018"), dateToEpoch("10-10-2018"), dateToEpoch("10-04-2019"),
  dateToEpoch("10-10-2019"), dateToEpoch("14-04-2020"), dateToEpoch("12-10-2020"), dateToEpoch("12-04-2021"),
dateToEpoch("11-10-2021"), dateToEpoch("11-04-2022"), dateToEpoch("10-10-2022"), dateToEpoch("11-04-2023")};*/

std::vector<time_t> semesterDates;

  char* date = argv[1];
  std::vector<Spot> euroStoxSpotspots = dbManager->getSpots("2016-01-19", "2017-01-19", "^STOXX50E");
  vectorToPnlVect(euroStoxSpotspots, euroStoxSpots);
  std::vector<Spot> spUsdSpotspots = dbManager->getSpots("2016-01-19", "2017-01-19", "^GSPC");
  vectorToPnlVect(spUsdSpotspots, spUsdSpots);
  std::vector<Spot> spAudSpotspots = dbManager->getSpots("2016-01-19", "2017-01-19", "^AXJO");
  vectorToPnlVect(spAudSpotspots, spAudSpots);
  std::vector<Spot> eurUsdSpotspots = dbManager->getSpots("2016-01-19", "2017-01-19", "EURUSD=X");
  vectorToPnlVect(eurUsdSpotspots, eurUsdSpots);
  std::vector<Spot> eurAudSpotspots = dbManager->getSpots("2016-01-19", "2017-01-19", "EURAUD=X");
  vectorToPnlVect(eurAudSpotspots, eurAudSpots);
  int dataSize = euroStoxSpots->size;
  int actigoSize = 5;
  double maturity = 1.0;
  //maturity to modify
  PnlMat* calibrationDataMatrix = pnl_mat_create_from_scalar(dataSize, actigoSize, 0.);
  pnl_mat_set_col(calibrationDataMatrix, euroStoxSpots, 0);
  pnl_mat_set_col(calibrationDataMatrix, spUsdSpots, 1);
  pnl_mat_set_col(calibrationDataMatrix, spAudSpots, 2);
  pnl_mat_set_col(calibrationDataMatrix, eurUsdSpots, 3);
  pnl_mat_set_col(calibrationDataMatrix, eurAudSpots, 4);

  Data *data = new Data(calibrationDataMatrix);
  double step = maturity / dataSize;
  Calibration *calibration = new Calibration(data, step);
  double euroStoxInitialSpot = dbManager->getSpot("2015-04-08", "^STOXX50E").getClose();
  double spUsdInitialSpot = dbManager->getSpot("2015-04-08", "^GSPC").getClose();
  double spAudInitialSpot = dbManager->getSpot("2015-04-08", "^AXJO").getClose();
  Actigo *actigo = new Actigo(maturity, 16, actigoSize, euroStoxInitialSpot, spUsdInitialSpot, spAudInitialSpot);
  //Create the BlackScholesModel
  double rEur = 0.04;
  //recuprate Initial Spots
  PnlVect* initialSpotsEuro = pnl_vect_create_from_scalar(actigoSize, 0);
  BlackScholesModel *bsm = new BlackScholesModel(actigoSize, rEur, calibration->getCorrelationsMatrix(),
                                            calibration->getVolatilities(), initialSpotsEuro);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, 0.01, 50000);
  PnlVect *delta = pnl_vect_create_from_scalar(actigoSize, 0.);
  double price = 0.;
  time_t epochDate = dateToEpoch(date);
  std::vector<time_t> rightDates = getRightDates(epochDate, semesterDates);
  PnlMat* past = pnl_mat_create_from_scalar(rightDates.size(), actigoSize, 0.);
  getPastData(dbManager, past, rightDates);
  double convertedDate;
  mc->rebalanceAtSpecificDate(past,  convertedDate, delta, price);
  pnl_vect_print(delta);
  std::cout << std::endl;
  std::cout << price << std::endl;

}
