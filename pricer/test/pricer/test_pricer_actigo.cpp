#include "Calibration.hpp"
#include "gtest/gtest.h"
#include "spot.hpp"
#include "DataBaseManager.hpp"
#include "Data.hpp"
#include "Actigo.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Option.hpp"
#include "pricer_utils.hpp"
#include "time_utils.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class ActigoPricerTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            // code called before each test (beginning)
        }

        virtual void TearDown() {
            // code called before each test destructor (ending)
        }
};

TEST_F(ActigoPricerTest, priceAtZero) {
  int mcSamplesNb = 5000; // Monte Carlo samples number
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  std::vector<time_t> semesterDates {
      1428624000,
      1444608000,
      1460332800,
      1476057600,
      1491782400,
      1507593600,
      1523318400,
      1539129600,
      1554854400,
      1570665600,
      1586822400,
      1602460800,
      1618185600,
      1633910400,
      1649635200,
      1665360000,
      1681171200
  };

  time_t date = 1428624000 ;
  time_t oneYearBeforeDate = date -  365 * 24 * 3600;

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

  // Resize spots
  pnl_vect_resize(euroStoxSpots, minSize);
  pnl_vect_resize(spUsdSpots, minSize);
  pnl_vect_resize(spAudSpots, minSize);
  pnl_vect_resize(eurUsdSpots, minSize);
  pnl_vect_resize(eurAudSpots, minSize);
  int dataSize = euroStoxSpots->size;
  int actigoSize = 5;
  double calibrationMaturity = 1;

  double rEur = 0.0073;
  double rUsd = 0.02597;
  double rAud = 0.02703;

  // Maturity to modify
  PnlMat* calibrationDataMatrix = pnl_mat_create_from_scalar(dataSize, actigoSize, 0);
  pnl_mat_set_col(calibrationDataMatrix, euroStoxSpots, 0);
  pnl_mat_set_col(calibrationDataMatrix, spUsdSpots, 1);
  pnl_mat_set_col(calibrationDataMatrix, spAudSpots, 2);
  pnl_mat_set_col(calibrationDataMatrix, eurUsdSpots, 3);
  pnl_mat_set_col(calibrationDataMatrix, eurAudSpots, 4);

  Data *data = new Data(calibrationDataMatrix, rEur, rUsd, rAud);
  double step = calibrationMaturity / dataSize;
  Calibration *calibration = new Calibration(data, step);

  // Recuperate Initial Spots
  Spot euroStoxInitialSpot = dbManager->getSpot(1428624000, "^STOXX50E");
  Spot spUsdInitialSpot = dbManager->getSpot(1428624000, "^GSPC");
  Spot spAudInitialSpot = dbManager->getSpot(1428624000, "^AXJO");
  Spot eurUsdInitialSpot = dbManager->getSpot(1428624000, "EURUSD=X");
  Spot eurAudInitialSpot = dbManager->getSpot(1428624000, "EURAUD=X");

  double euroStoxInitialPrice = euroStoxInitialSpot.getClose();
  double spUsdInitialPrice = spUsdInitialSpot.getClose();
  double spAudInitialPrice = spAudInitialSpot.getClose();
  double eurUsdInitialPrice = eurUsdInitialSpot.getClose();
  double eurAudInitialPrice = eurAudInitialSpot.getClose();

  double maturity = 8;

  Actigo *actigo = new Actigo(
              maturity, 16, actigoSize,
              euroStoxInitialPrice, spUsdInitialPrice, spAudInitialPrice,
              rEur, rUsd, rAud);

  // Create the BlackScholesModel

  double actuParam = exp(-rEur*maturity);
  PnlVect* initialPricesEuro = pnl_vect_create_from_scalar(actigoSize, 0);
  spUsdInitialPrice *= eurUsdInitialPrice;
  spAudInitialPrice *= eurAudInitialPrice;
  eurUsdInitialPrice *= actuParam;
  eurAudInitialPrice *= actuParam;
  LET(initialPricesEuro, 0) = euroStoxInitialPrice;
  LET(initialPricesEuro, 1) = spUsdInitialPrice;
  LET(initialPricesEuro, 2) = spAudInitialPrice;
  LET(initialPricesEuro, 3) = eurUsdInitialPrice;
  LET(initialPricesEuro, 4) = eurAudInitialPrice;

  BlackScholesModel *bsm = new BlackScholesModel(
              actigoSize, rEur, calibration->getCorrelationsMatrix(),
              calibration->getVolatilities(), initialPricesEuro);

  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, 0.01, mcSamplesNb);
  PnlVect *delta = pnl_vect_create_from_scalar(actigoSize, 0);
  double price = 0;
  std::vector<time_t> rightDates = getRightDates(date, semesterDates);
  PnlMat* past = pnl_mat_create_from_scalar(rightDates.size(), actigoSize, 0);
  getPastData(dbManager, past, rightDates, rUsd, rAud);

  time_t dateDifference = date - 1428624000;
  double convertedDate = (double)dateDifference/(365 * 24 * 3600);

  if ( convertedDate > 8)
      convertedDate = 8;

  mc->rebalanceAtSpecificDate(past,  convertedDate, delta, price);
  EXPECT_TRUE(price >= 90*exp(-rEur*8.0));
  EXPECT_TRUE(price <= 100);

  // Free memory
  delete data;
  delete calibration;
  delete actigo;
  delete bsm;
  delete mc;

  pnl_vect_free(&euroStoxSpots);
  pnl_vect_free(&spUsdSpots);
  pnl_vect_free(&spAudSpots);
  pnl_vect_free(&eurUsdSpots);
  pnl_vect_free(&eurAudSpots);
  pnl_vect_free(&initialPricesEuro);
  pnl_rng_free(&rng);
  pnl_vect_free(&delta);
  pnl_mat_free(&past);
  }

  TEST_F(ActigoPricerTest, priceAtDate) {
    int mcSamplesNb = 5000; // Monte Carlo samples number
    DataBaseManager *dbManager = DataBaseManager::getDbManager();
    std::vector<time_t> semesterDates {
        1428624000,
        1444608000,
        1460332800,
        1476057600,
        1491782400,
        1507593600,
        1523318400,
        1539129600,
        1554854400,
        1570665600,
        1586822400,
        1602460800,
        1618185600,
        1633910400,
        1649635200,
        1665360000,
        1681171200
    };

    time_t date = 1460332800 ;
    time_t oneYearBeforeDate = date -  365 * 24 * 3600;

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

    // Resize spots
    pnl_vect_resize(euroStoxSpots, minSize);
    pnl_vect_resize(spUsdSpots, minSize);
    pnl_vect_resize(spAudSpots, minSize);
    pnl_vect_resize(eurUsdSpots, minSize);
    pnl_vect_resize(eurAudSpots, minSize);
    int dataSize = euroStoxSpots->size;
    int actigoSize = 5;
    double calibrationMaturity = 1;

    double rEur = 0.0073;
    double rUsd = 0.02597;
    double rAud = 0.02703;

    // Maturity to modify
    PnlMat* calibrationDataMatrix = pnl_mat_create_from_scalar(dataSize, actigoSize, 0);
    pnl_mat_set_col(calibrationDataMatrix, euroStoxSpots, 0);
    pnl_mat_set_col(calibrationDataMatrix, spUsdSpots, 1);
    pnl_mat_set_col(calibrationDataMatrix, spAudSpots, 2);
    pnl_mat_set_col(calibrationDataMatrix, eurUsdSpots, 3);
    pnl_mat_set_col(calibrationDataMatrix, eurAudSpots, 4);

    Data *data = new Data(calibrationDataMatrix, rEur, rUsd, rAud);
    double step = calibrationMaturity / dataSize;
    Calibration *calibration = new Calibration(data, step);

    // Recuperate Initial Spots
    Spot euroStoxInitialSpot = dbManager->getSpot(1428624000, "^STOXX50E");
    Spot spUsdInitialSpot = dbManager->getSpot(1428624000, "^GSPC");
    Spot spAudInitialSpot = dbManager->getSpot(1428624000, "^AXJO");
    Spot eurUsdInitialSpot = dbManager->getSpot(1428624000, "EURUSD=X");
    Spot eurAudInitialSpot = dbManager->getSpot(1428624000, "EURAUD=X");

    double euroStoxInitialPrice = euroStoxInitialSpot.getClose();
    double spUsdInitialPrice = spUsdInitialSpot.getClose();
    double spAudInitialPrice = spAudInitialSpot.getClose();
    double eurUsdInitialPrice = eurUsdInitialSpot.getClose();
    double eurAudInitialPrice = eurAudInitialSpot.getClose();

    double maturity = 8;

    Actigo *actigo = new Actigo(
                maturity, 16, actigoSize,
                euroStoxInitialPrice, spUsdInitialPrice, spAudInitialPrice,
                rEur, rUsd, rAud);

    // Create the BlackScholesModel

    double actuParam = exp(-rEur*maturity);
    PnlVect* initialPricesEuro = pnl_vect_create_from_scalar(actigoSize, 0);
    spUsdInitialPrice *= eurUsdInitialPrice;
    spAudInitialPrice *= eurAudInitialPrice;
    eurUsdInitialPrice *= actuParam;
    eurAudInitialPrice *= actuParam;
    LET(initialPricesEuro, 0) = euroStoxInitialPrice;
    LET(initialPricesEuro, 1) = spUsdInitialPrice;
    LET(initialPricesEuro, 2) = spAudInitialPrice;
    LET(initialPricesEuro, 3) = eurUsdInitialPrice;
    LET(initialPricesEuro, 4) = eurAudInitialPrice;

    BlackScholesModel *bsm = new BlackScholesModel(
                actigoSize, rEur, calibration->getCorrelationsMatrix(),
                calibration->getVolatilities(), initialPricesEuro);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, 0.01, mcSamplesNb);
    PnlVect *delta = pnl_vect_create_from_scalar(actigoSize, 0);
    double price = 0;
    std::vector<time_t> rightDates = getRightDates(date, semesterDates);
    PnlMat* past = pnl_mat_create_from_scalar(rightDates.size(), actigoSize, 0);
    getPastData(dbManager, past, rightDates, rUsd, rAud);

    time_t dateDifference = date - 1428624000;
    double convertedDate = (double)dateDifference/(365 * 24 * 3600);

    if ( convertedDate > 8)
        convertedDate = 8;

    mc->rebalanceAtSpecificDate(past,  convertedDate, delta, price);
    EXPECT_TRUE(price >= 90*exp(-rEur*(8.0 - convertedDate)));
    EXPECT_TRUE(price <= 100);

    // Free memory
    delete data;
    delete calibration;
    delete actigo;
    delete bsm;
    delete mc;

    pnl_vect_free(&euroStoxSpots);
    pnl_vect_free(&spUsdSpots);
    pnl_vect_free(&spAudSpots);
    pnl_vect_free(&eurUsdSpots);
    pnl_vect_free(&eurAudSpots);
    pnl_vect_free(&initialPricesEuro);
    pnl_rng_free(&rng);
    pnl_vect_free(&delta);
    pnl_mat_free(&past);
    }
