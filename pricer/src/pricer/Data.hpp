#ifndef DATA_H
#define	DATA_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Option.hpp"
#include "../quote/quote.hpp"

/**
 * @brief Data class
 */

class Data{
public:
    PnlVect* euroStoxSpots;  //euroStox spAudSpots
    PnlVect* spUsdSpots; // S&P 500 spots
    PnlVect* spAudSpots; // S&P 200 spots
    PnlVect* eurUsd; // USD/EUR rates
    PnlVect* eurAud; // AUD/EUR rates
    double rEur; // European free rate
    double rUsd; // American free rate
    double rAud; // Australian free rate
    PnlMat* historicalDataMatrix; // Matrix containing hsitorical extrracted Data from the begenning of Actigo to current Day
    PnlMat* historicalDataMatrixEuro;
    PnlMat* completeDataMatrix; // Matrix containing historical data plus simulated Data

    /**
      * @brief Calibration constructor
      */
    Data();

    /**
      * @brief Data constructor.
      * @brief Extracts historical market data for days between debutDay and currentDay
      * @brief and store them
      * @param debutDate is the data's first day "YYYY-MM-DD" format
      * @param currentDate is the data's last day
      */
    Data(const char* debutDate, const char* currentDate);

    /**
      * @brief Data constructor.
      * @brief construct the data from an already filled matrix
      * @param matrixData matrix 5x5 containing the market data
      */
    Data(PnlMat *matrixData);

    /**
      * @brief Calibration destructor
      */
    ~Data();

    /**
      * @brief Data constructor.
      * @brief copy constructor
      * @param orig data to copy
      */
    Data(const Data& orig);

    /**
      * @param initialSpots
      * @brief fill the input vector with the initialSpots
      */
    void getInitialSpots(PnlVect *initialSpots);

    /**
      * @param initialSpots
      * @brief fill the input vector with the initialSpots converted to euro
      */
    void getInitialSpotsEuro(PnlVect *initialSpots);

    /**
      * @brief fill the input vector with current spots which is the last line of the historical data matrix
      * @param toDaySpots
      */
    void getTodaySpots(PnlVect *toDaySpots);

    /**
      * @param remainingDates day between today and acitgo's maturity date
      * @param option Actigo
      * @param currentSpots toDaySpots
      * @param volatilities vector of volatilities after calibrating the historical data
      * @param correlations correlations' matrix after calibrating historical data
      * @param trends trends' vector after calibrating historical data
      * @param rng random simulator
      * @brief fill completeDataMatrix with historicalDataMatrix plus simulated data from today to actigo's matrity date
      */
    void completeData(int remainingDates, Option *option, PnlVect *currentSpots,
                      PnlVect *volatilities, PnlMat *correlations, PnlVect* trends, PnlRng* rng);

    void getDataAtRebalancingDates(PnlMat* path, int rebalancingFrequency);

    void getZeroCoupon(PnlVect *exchangeRate, double r, double maturity);

    //void vectorToPnlVect(vector<spot> spots, PnlVect* spotsVect);
};

#endif /* DATA_H */
