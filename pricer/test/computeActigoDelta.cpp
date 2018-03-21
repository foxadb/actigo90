#include "spot.hpp"
#include "DataBaseManager.hpp"
#include "Data.hpp"
#include "Actigo.hpp"
#include "Calibration.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Option.hpp"
#include "pricer_utils.hpp"
#include "time_utils.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <ctime>

int main(int argc, char **argv){
    DataBaseManager *dbManager = DataBaseManager::getDbManager();

std::vector<time_t> semesterDatesT {dateToEpoch("2015-04-08"), dateToEpoch("2015-10-12"), dateToEpoch("2016-04-11"),
 dateToEpoch("2016-10-10"), dateToEpoch("2017-04-10"),
    dateToEpoch("2017-10-10"), dateToEpoch("2018-04-10"), dateToEpoch("2018-10-10"), dateToEpoch("2019-04-10"),
  dateToEpoch("2019-10-10"), dateToEpoch("2020-04-14"), dateToEpoch("2020-10-12"), dateToEpoch("2021-04-12"),
dateToEpoch("2021-10-11"), dateToEpoch("2022-04-11"), dateToEpoch("2022-10-10"), dateToEpoch("2023-04-11")};

std::vector<time_t> semesterDates {1428451200, 1444608000, 1460332800, 1476057600,
                                    1491782400, 1507593600, 1523318400, 1539129600,
                                  1554854400, 1570665600, 1586822400, 1602460800,
                                1618185600, 1633910400, 1649635200, 1665360000, 1681171200};
  time_t date = atoi(argv[1]);
  time_t oneYearBeforeDate = date -  365*24*3600;

  std::vector<Spot> euroStoxSpotspots = dbManager->getSpots(oneYearBeforeDate, date, "^STOXX50E");
  PnlVect* euroStoxSpots = pnl_vect_create_from_scalar(euroStoxSpotspots.size(), 0.);
  vectorToPnlVect(euroStoxSpotspots, euroStoxSpots);
  std::vector<Spot> spUsdSpotspots = dbManager->getSpots(oneYearBeforeDate, date, "^GSPC");
  PnlVect* spUsdSpots = pnl_vect_create_from_scalar(spUsdSpotspots.size(), 0.);
  vectorToPnlVect(spUsdSpotspots, spUsdSpots);
  std::vector<Spot> spAudSpotspots = dbManager->getSpots(oneYearBeforeDate, date, "^AXJO");
  PnlVect* spAudSpots = pnl_vect_create_from_scalar(spAudSpotspots.size(), 0.);
  vectorToPnlVect(spAudSpotspots, spAudSpots);
  std::vector<Spot> eurUsdSpotspots = dbManager->getSpots(oneYearBeforeDate, date, "EURUSD=X");
  PnlVect* eurUsdSpots = pnl_vect_create_from_scalar(eurUsdSpotspots.size(), 0.);
  vectorToPnlVect(eurUsdSpotspots, eurUsdSpots);
  std::vector<Spot> eurAudSpotspots = dbManager->getSpots(oneYearBeforeDate, date, "EURAUD=X");
  PnlVect* eurAudSpots = pnl_vect_create_from_scalar(eurAudSpotspots.size(), 0.);
  vectorToPnlVect(eurAudSpotspots, eurAudSpots);
  //int dataSize = euroStoxSpots->size;
  double minSize = MIN(euroStoxSpots->size, spUsdSpots->size);
  minSize = MIN(minSize, spAudSpots->size);
  minSize = MIN(minSize, eurUsdSpots->size);
  minSize = MIN(minSize, eurAudSpots->size);
  //resize spots
  pnl_vect_resize(euroStoxSpots, minSize);
  pnl_vect_resize(spUsdSpots, minSize);
  pnl_vect_resize(spAudSpots, minSize);
  pnl_vect_resize(eurUsdSpots, minSize);
  pnl_vect_resize(eurAudSpots, minSize);
  int dataSize = euroStoxSpots->size;
  int actigoSize = 5;
  double calibrationMaturity = 1.0;
  //maturity to modify
  PnlMat* calibrationDataMatrix = pnl_mat_create_from_scalar(dataSize, actigoSize, 0.);
  pnl_mat_set_col(calibrationDataMatrix, euroStoxSpots, 0);
  pnl_mat_set_col(calibrationDataMatrix, spUsdSpots, 1);
  pnl_mat_set_col(calibrationDataMatrix, spAudSpots, 2);
  pnl_mat_set_col(calibrationDataMatrix, eurUsdSpots, 3);
  pnl_mat_set_col(calibrationDataMatrix, eurAudSpots, 4);

  Data *data = new Data(calibrationDataMatrix);
  double step = calibrationMaturity / dataSize;
  Calibration *calibration = new Calibration(data, step);
  double euroStoxInitialSpot = dbManager->getSpot(1428451200, "^STOXX50E").getClose();
  double spUsdInitialSpot = dbManager->getSpot(1428451200, "^GSPC").getClose();
  double spAudInitialSpot = dbManager->getSpot(1428451200, "^AXJO").getClose();
  double maturity = 8.0;
  Actigo *actigo = new Actigo(maturity, 16, actigoSize, euroStoxInitialSpot, spUsdInitialSpot, spAudInitialSpot);
  //Create the BlackScholesModel
  double rEur = 0.04;
  //recuprate Initial Spots
  double actuParam = exp(-rEur*maturity);
  PnlVect* initialSpotsEuro = pnl_vect_create_from_scalar(actigoSize, 0);
  double eurUsdInitialSpot = dbManager->getSpot(1428451200, "EURUSD=X").getClose() ;
  spUsdInitialSpot *= eurUsdInitialSpot;
  double eurAudInitialSpot = dbManager->getSpot(1428451200, "EURAUD=X").getClose() ;
  spAudInitialSpot *= eurAudInitialSpot;
  eurUsdInitialSpot *= actuParam;
  eurAudInitialSpot *= actuParam;
  LET(initialSpotsEuro,0) = euroStoxInitialSpot;
  LET(initialSpotsEuro,1) = spUsdInitialSpot;
  LET(initialSpotsEuro,2) = spAudInitialSpot;
  LET(initialSpotsEuro,3) = eurUsdInitialSpot;
  LET(initialSpotsEuro,4) = eurAudInitialSpot;
  BlackScholesModel *bsm = new BlackScholesModel(actigoSize, rEur, calibration->getCorrelationsMatrix(),
                                            calibration->getVolatilities(), initialSpotsEuro);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, 0.01, 5);
  PnlVect *delta = pnl_vect_create_from_scalar(actigoSize, 0.);
  double price = 0.;
  std::vector<time_t> rightDates = getRightDates(date, semesterDates);
  PnlMat* past = pnl_mat_create_from_scalar(rightDates.size(), actigoSize, 0.);
  getPastData(dbManager, past, rightDates);
  pnl_mat_print(past);

  time_t dateDifference = date - 1428451200;
  double convertedDate = (double)dateDifference/(365*24*3600);
  if ( convertedDate > 8.0)
    convertedDate = 8. ;
  mc->rebalanceAtSpecificDate(past,  convertedDate, delta, price);
  pnl_vect_print(delta);
  std::cout << price << std::endl;
}
