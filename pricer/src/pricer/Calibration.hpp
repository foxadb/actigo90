/**
 * @file Calibration.hpp
 * @version 0.1
 * @date 26 janvier 2018
 *
 *
 */
#include <cstdio>
#include "Data.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/**
 * @brief Calibration class
 */
class Calibration {
private:
  PnlVect *volatilities; // volatilities vector associated to data. Empty by default.
  PnlMat *correlations;  // correlations matrix associated to data. Empty by default.
  PnlVect *trends; // trends vector associated to data. Empty by default.
  double step; // step between data dates


public:
  /**
  * @brief Calibration constructor
  */
  Calibration();

  /**
  * @brief Calibration constructor.
  * @brief Estimates the trends, volatilities and correlations corresponding to data
  * @brief and stores them.
  * @param data contains the data that we wish to calibrate.
  * @param step the time separating two dates at which data has been retrieved.
  */
  Calibration(Data *data, double step);

  /**
  * @brief Calibration destructor
  */
  ~Calibration();

  /**
  * @param x
  * @return the volatility of the log profitability associated to x.
  */
  double estimate_volatility(PnlVect *x);

  /**
  * @param x
  * @param y
  * @return the correlation between the log profitability associated to x and y
  */
  double estimate_correlation(PnlVect *x, PnlVect *y);

  /**
  * @param x
  * @return the expectation of the log profitability associated to x.
  * @brief Attention: if you want to estimate the real trend (in a BlackScholesModel)
  * make sure to add a term sigma^2 / 2.
  */
  double estimate_trend(PnlVect *x);

  /**
  * @return a PnlVect containing the volatilities of the data with which
  * Calibration was instanciated.
  * If Calibration was instanciated without data then the returned vector will be empty.
  */
  PnlVect* getVolatilities();

  /**
  * @return a PnlMat containing the correlations of the data with which
  * Calibration was instanciated
  * If Calibration was instanciated without data then the returned matrix will be empty;
  */
  PnlMat* getCorrelationsMatrix();

  /**
  * @return a PnlVect containing the volatilities of the data with which
  * Calibration was instanciated.
  * If Calibration was instanciated without data then the returned vector will be empty.
  */
  PnlVect* getTrends();
};
