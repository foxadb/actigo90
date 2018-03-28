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

int main(int argc, char** argv) {
    if (argc == 4) {
        // Ending hedging date
        time_t endingDate = std::atoi(argv[1]);

        // Rebalancing frequency
        int rebalancingFrequency = std::atoi(argv[2]);

        // Monte Carlo samples number
        int mcSamplesNb = std::atoi(argv[3]);

        std::vector<time_t> semesterDates {
            1428451200, 1444608000, 1460332800, 1476057600,
            1491782400, 1507593600, 1523318400, 1539129600,
            1554854400, 1570665600, 1586822400, 1602460800,
            1618185600, 1633910400, 1649635200, 1665360000,
            1681171200
        };

        DataBaseManager *dbManager = DataBaseManager::getDbManager();

        //deleting deltas and prices in case someone else has already computed
        // at those dates
        dbManager->clearDeltas();
        dbManager->clearPrices(); 

        time_t last_year = endingDate - 365 * 24 * 3600;
        std::vector<Spot> euroStoxSpotspots = dbManager->getSpots(last_year, endingDate, "^STOXX50E");

        PnlVect* euroStoxSpots = pnl_vect_create_from_scalar(euroStoxSpotspots.size(), 0);
        vectorToPnlVect(euroStoxSpotspots, euroStoxSpots);
        std::vector<Spot> spUsdSpotspots = dbManager->getSpots(last_year, endingDate, "^GSPC");

        PnlVect* spUsdSpots = pnl_vect_create_from_scalar(spUsdSpotspots.size(), 0);
        vectorToPnlVect(spUsdSpotspots, spUsdSpots);
        std::vector<Spot> spAudSpotspots = dbManager->getSpots(last_year, endingDate, "^AXJO");

        PnlVect* spAudSpots = pnl_vect_create_from_scalar(spAudSpotspots.size(), 0);
        vectorToPnlVect(spAudSpotspots, spAudSpots);
        std::vector<Spot> eurUsdSpotspots = dbManager->getSpots(last_year, endingDate, "EURUSD=X");

        PnlVect* eurUsdSpots = pnl_vect_create_from_scalar(eurUsdSpotspots.size(), 0);
        vectorToPnlVect(eurUsdSpotspots, eurUsdSpots);
        std::vector<Spot> eurAudSpotspots = dbManager->getSpots(last_year, endingDate, "EURAUD=X");

        PnlVect* eurAudSpots = pnl_vect_create_from_scalar(eurAudSpotspots.size(), 0);
        vectorToPnlVect(eurAudSpotspots, eurAudSpots);

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

        // Free risk rates
        double rEur = 0.0075;
        double rUsd = 0.028;
        double rAud = 0.026;

        // Maturity to modify
        PnlMat* calibrationDataMatrix = pnl_mat_create_from_scalar(dataSize, actigoSize, 0);
        pnl_mat_set_col(calibrationDataMatrix, euroStoxSpots, 0);
        pnl_mat_set_col(calibrationDataMatrix, spUsdSpots, 1);
        pnl_mat_set_col(calibrationDataMatrix, spAudSpots, 2);
        pnl_mat_set_col(calibrationDataMatrix, eurUsdSpots, 3);
        pnl_mat_set_col(calibrationDataMatrix, eurAudSpots, 4);

        Data *data = new Data(calibrationDataMatrix, rEur, rUsd, rAud);
        double step = calibrationMaturity / dataSize;

        // Create the calibration object
        Calibration *calibration = new Calibration(data, step);

        double euroStoxInitialSpot = dbManager->getSpot(1428451200, "^STOXX50E").getClose();
        double spUsdInitialSpot = dbManager->getSpot(1428451200, "^GSPC").getClose();
        double spAudInitialSpot = dbManager->getSpot(1428451200, "^AXJO").getClose();
        double maturity = 8;

        // Create Actigo
        Actigo *actigo = new Actigo(maturity, 16, actigoSize,
                                    euroStoxInitialSpot, spUsdInitialSpot, spAudInitialSpot,
                                    rEur, rUsd, rAud);

        // Recuprate Initial Spots
        double actuParam = exp(-rEur * maturity);
        PnlVect* initialSpotsEuro = pnl_vect_create_from_scalar(actigoSize, 0);
        double eurUsdInitialSpot = dbManager->getSpot(1428451200, "EURUSD=X").getClose() ;
        spUsdInitialSpot *= eurUsdInitialSpot;
        double eurAudInitialSpot = dbManager->getSpot(1428451200, "EURAUD=X").getClose() ;
        spAudInitialSpot *= eurAudInitialSpot;

        // TODO change zeros coupons so as to use the correct free risk rates
        eurUsdInitialSpot *= actuParam;
        eurAudInitialSpot *= actuParam;
        LET(initialSpotsEuro, 0) = euroStoxInitialSpot;
        LET(initialSpotsEuro, 1) = spUsdInitialSpot;
        LET(initialSpotsEuro, 2) = spAudInitialSpot;
        LET(initialSpotsEuro, 3) = eurUsdInitialSpot;
        LET(initialSpotsEuro, 4) = eurAudInitialSpot;

        // Create the BlackScholesModel
        BlackScholesModel *bsm = new BlackScholesModel(
                    actigoSize, rEur, calibration->getCorrelationsMatrix(),
                    calibration->getVolatilities(), initialSpotsEuro);

        PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));

        // Create the MonteCarlo simulator
        MonteCarlo *mc = new MonteCarlo(bsm, actigo, rng, 0.01, mcSamplesNb);

        PnlVect* currentDelta = pnl_vect_create_from_scalar(actigoSize, 0);
        PnlVect* previousDelta = pnl_vect_create_from_scalar(actigoSize, 0);

        std::vector<time_t> rightDates;
        double actigoPrice = 0;
        double hedgingPrice = 0;
        double freeRiskPart = 0;
        double riskedPart = 0;

        PnlVect* currentSpots = pnl_vect_create_from_scalar(actigoSize, 0);

        // Rebalancing until endingDate
        for (time_t currentDate = 1428451200; currentDate <= endingDate;
             currentDate += rebalancingFrequency * 86400) {
            // Timer start
            std::time_t start = std::clock();

            rightDates = getRightDates(currentDate, semesterDates);
            PnlMat* past = pnl_mat_create_from_scalar(rightDates.size(), actigoSize, 0);
            getPastData(dbManager, past, rightDates, rUsd, rAud);

            time_t dateDifference = currentDate - 1428451200;

            double convertedDate = (double) dateDifference / (365 * 24 * 3600);
            if (convertedDate > 8) {
                convertedDate = 8;
            }

            LET(currentSpots, 0) = getLastAvailableSpot(dbManager, "^STOXX50E", currentDate);
            LET(currentSpots, 1) = getLastAvailableSpot(dbManager, "^GSPC", currentDate);
            LET(currentSpots, 2) = getLastAvailableSpot(dbManager, "^AXJO", currentDate);
            LET(currentSpots, 3) = getLastAvailableSpot(dbManager, "EURUSD=X", currentDate);
            LET(currentSpots, 4) = getLastAvailableSpot(dbManager, "EURAUD=X", currentDate);

            mc->rebalanceAtSpecificDate(past, convertedDate, currentDelta, actigoPrice);

            // Send deltas to database
            dbManager->postDelta(GET(currentDelta, 0), currentDate, "^STOXX50E");
            dbManager->postDelta(GET(currentDelta, 1), currentDate, "^GSPC");
            dbManager->postDelta(GET(currentDelta, 2), currentDate, "^AXJO");
            dbManager->postDelta(GET(currentDelta, 3), currentDate, "EURUSD=X");
            dbManager->postDelta(GET(currentDelta, 4), currentDate, "EURAUD=X");

            riskedPart = pnl_vect_scalar_prod(currentSpots, currentDelta);

            if (currentDate == 1428451200) {
                freeRiskPart = actigoPrice - riskedPart;
            } else {
                freeRiskPart = freeRiskPart * exp(rEur * (rebalancingFrequency / 365.0))
                        - riskedPart + pnl_vect_scalar_prod(previousDelta, currentSpots);
            }

            hedgingPrice = freeRiskPart + riskedPart;

            // Send price to database
            dbManager->postPrice(currentDate, actigoPrice, hedgingPrice);

            // Clone current delta for next iteration
            pnl_vect_clone(previousDelta, currentDelta);

            // Timer end
            double duration = (double)(std::clock() - start) / CLOCKS_PER_SEC;

            std::cout << "Date: " << epochToDate(currentDate)
                      << ", Actigo: " << actigoPrice
                      << ", Hedging: " << hedgingPrice
                      << ", Error: " << hedgingPrice - actigoPrice
                      << ", Timer: " << duration << " s"
                      << std::endl;

            // Free past matrix
            pnl_mat_free(&past);
        }

        // Free memory
        pnl_rng_free(&rng);

        pnl_vect_free(&euroStoxSpots);
        pnl_vect_free(&spUsdSpots);
        pnl_vect_free(&spAudSpots);
        pnl_vect_free(&eurUsdSpots);
        pnl_vect_free(&eurAudSpots);
        pnl_vect_free(&initialSpotsEuro);
        pnl_vect_free(&currentSpots);
        pnl_vect_free(&currentDelta);
        pnl_vect_free(&previousDelta);

        delete data;
        delete actigo;
        delete calibration;
        delete bsm;
        delete mc;
    }
}
