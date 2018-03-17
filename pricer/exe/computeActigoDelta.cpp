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

int main(int argc, char **argv){
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  PnlVect* euroStoxSpots;
  PnlVect* spUsdSpots;
  PnlVect* spAudSpots;
  PnlVect* eurUsdSpots;
  PnlVect* eurAudSpots;

  std::vector<ctime> semesterDates {dateToEpoch("12-10-2015"), dateToEpoch("11-04-2016"), dateToEpoch("10-10-2016"), dateToEpoch("10-04-2017")
    dateToEpoch("10-10-2017"), dateToEpoch("10-04-2018"), dateToEpoch("10-10-2018"), dateToEpoch("10-04-2019"),
  dateToEpoch("10-10-2019"), dateToEpoch("14-04-2020"), dateToEpoch("12-10-2020"), dateToEpoch("12-04-2021"),
dateToEpoch("11-10-2021"), dateToEpoch("11-04-2022"), dateToEpoch("10-10-2022"), dateToEpoch("11-04-2023")};

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
  PnlMat* calibrationDataMatrix = pnl_mat_create_from_scalar(dataSize, actigoSize);
  pnl_mat_set_col(dataMatrix, euroStoxSpots, 0);
  pnl_mat_set_col(dataMatrix, spUsdSpots, 1);
  pnl_mat_set_col(dataMatrix, spAudSpots, 2);
  pnl_mat_set_col(dataMatrix, eurUsdSpots, 3);
  pnl_mat_set_col(dataMatrix, eurAudSpots, 4);

  Data *data = new Data(dataMatrix);
  double step = maturity / dataSize;
  Calibration *calibration = new Calibration(data, step);
  double euroStoxInitialSpot = dbManager->getSpot("2015-04-08", "^STOXX50E").getClose();
  double spUsdInitialSpot = dbManager->getSpot("2015-04-08", "^GSPC").getClose();
  double spAudInitialSpot = dbManager->getSpot("2015-04-08", "^AXJO").getClose();
  Actigo *actigo = new Actigo(maturity, 16, size, euroStoxInitialSpot, spUsdInitialSpot, spAudInitialSpot);
  //Create the BlackScholesModel
  double rEur = 0.04;
  //recuprate Initial Spots
  BlackScholesModel *bsm = new BlackScholesModel(size, rEur, calibration->getCorrelationsMatrix(),
                                            calibration->getVolatilities(), initialSpotsEuro);
  MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, 0.01, 50000);
  PnlVect *delta = pnl_vect_create(actigoSize, 0.);
  double price = 0.;
  ctime epochDate = dateToEpoch(date);
  std::vector<ctime> rightDates = getRightDates(epochDate, semesterDates);
  PnlMat* past = pnl_mat_create_from_scalar(rightDates.size(), 0.);
  getPastData(dbManager, past, rightDates);
  mc->rebalanceAtSpecificDate(past,  date, delta, price);
  pnl_vect_print(delta);
  cout << endl;
  cout << price << endl;

}

void vectorToPnlVect(vector<spot> spots, PnlVect* spotsVect){
  spotsVect = pnl_vect_create_from_scalar(spots.size(), 0.);
  int count = 0;
  for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it){
     LET(spotsVect, count) = it.getClose();
     count++;
  }
}

std::vector<ctime> getRightDates(ctime today, std::vector<ctime> semetrialDates){
  std::vector<ctime> rightDates;
  for (std::vector<ctime>::iterator it = semesterDates.begin(); it != semesterDates.end(); ++it){
    if ( it >= today)
      break;
    rightDates.push_back(it);
  }
  rightDates.push_back(today);
  return rightDates;
}

void getPastData(DataBaseManager dbm, PnlMat* past, std::vector<ctime> dates){
  int count = 0;
  PnlVect* euroStoxSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* spUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* spAudSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* eurUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  PnlVect* eurAudSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
  for (std::vector<ctime>::iterator it = semesterDates.begin(); it != semesterDates.end(); ++it){
      LET(euroStoxSpots, count) = dbManager->getSpote(epochToDate(it), "^STOXX50E").getClose();
      LET(spUsdSpots, count) = dbManager->getSpote(epochToDate(it), "^GSPC").getClose();
      LET(spAudSpots, count) = dbManager->getSpote(epochToDate(it), "^AXJO").getClose();
      LET(eurUsdSpots, count) = dbManager->getSpote(epochToDate(it), "EURUSD=X").getClose();
      LET(eurAudSpots, count) = dbManager->getSpote(epochToDate(it), "EURAUD=X").getClose();
      count++;
  }
  pnl_vect_mult_vect_term (spUsdSpots, eurUsd);
  pnl_vect_mult_vect_term (spAudSpots, eurAud);

  //constructing zero coupons
  getZeroCoupon(eurUsd, rUsd, 8.0);
  getZeroCoupon(eurAud, rAud, 8.0);
  pnl_mat_set_col(past, euroStoxSpots, 0);
  pnl_mat_set_col(past, spUsdSpots, 1);
  pnl_mat_set_col(past, spAudSpots, 2);
  pnl_mat_set_col(past, eurUsdSpots, 3);
  pnl_mat_set_col(past, eurAudSpots, 4);
}
