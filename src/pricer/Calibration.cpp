#include "Calibration.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "../quote/quote.hpp"
#include "../quote/forex.hpp"

using namespace std;

Calibration::Calibration(const char *period1_, const char *period2_) {

  this->period1 = period1_;
  this->period2 = period2_;

  // volatility for euro stoxx 50
  Quote* quote1 = new Quote("^STOXX50E");
  quote1->getHistoricalSpots(period1, period2, "1d");
  sigma_1 = estimate_volatility(quote1->getCloseSpots());

  // volatility for S&P 500
  Quote* quote2 = new Quote("^GSPC");
  quote2->getHistoricalSpots(period1, period2, "1d");
  sigma_2 = estimate_volatility(quote2->getCloseSpots());

  // volatility for S&P ASX 200
  Quote* quote3 = new Quote("^AYQFN");
  quote3->getHistoricalSpots(period1, period2, "1d");
  sigma_3 = estimate_volatility(quote3->getCloseSpots());

  // volatility for USD/EUR
  Forex *forex1 = new Forex("USD", "EUR");
  forex1->getHistoricalSpots(period1, period2);
  sigma_x1 = estimate_volatility(forex1->getCloseSpots());

  // volatility for AUD/EUR
  Forex *forex2 = new Forex("AUD", "EUR");
  forex2->getHistoricalSpots(period1, period2);
  sigma_x2 = estimate_volatility(forex2->getCloseSpots());

  // correlations;
  rho_12 = estimate_correlation(quote1->getCloseSpots(), quote2->getCloseSpots());
  rho_13 = estimate_correlation(quote1->getCloseSpots(), quote3->getCloseSpots());
  rho_23 = estimate_correlation(quote2->getCloseSpots(), quote3->getCloseSpots());
  rho_1x1 = estimate_correlation(quote1->getCloseSpots(), forex1->getCloseSpots());
  rho_1x2 = estimate_correlation(quote1->getCloseSpots(), forex2->getCloseSpots());
  rho_2x1 = estimate_correlation(quote2->getCloseSpots(), forex1->getCloseSpots());
  rho_2x2 = estimate_correlation(quote2->getCloseSpots(), forex2->getCloseSpots());
  rho_3x1 = estimate_correlation(quote3->getCloseSpots(), forex1->getCloseSpots());
  rho_3x2 = estimate_correlation(quote3->getCloseSpots(), forex2->getCloseSpots());
  rho_x1x2 = estimate_correlation(forex1->getCloseSpots(), forex2->getCloseSpots());

  delete quote1, quote2, quote3, forex1, forex2;
}


double Calibration::estimate_correlation(PnlVect *x, PnlVect *y){

  int size = min(x->size, y->size);

  PnlVect *log_rent_x = pnl_vect_create(size -1);
  PnlVect *log_rent_y = pnl_vect_create(size -1);

  for (int i =0; i<size-1; i++){
    LET(log_rent_x, i) = log(GET(x, i+1) / GET(x, i));
    LET(log_rent_y, i) = log(GET(y, i+1) / GET(y, i));
  }

  double x_mean, y_mean = 0.0;

  for (int i = 0; i < size; i++){
    x_mean += GET(log_rent_x, i);
    y_mean += GET(log_rent_y, i);
  }

  x_mean /= size - 1,
  y_mean /= size - 1;

  double covariance, x_var, y_var = 0.0;

  for (int i = 0; i < size -1; i++){
    covariance += (GET(log_rent_x,i) - x_mean) * (GET(log_rent_y,i) - y_mean);
    x_var += pow(GET(log_rent_x,i) - x_mean, 2);
    y_var += pow(GET(log_rent_y,i) - y_mean, 2);
  }

  pnl_vect_free(&log_rent_x);
  pnl_vect_free(&log_rent_y);

  if (x_var == 0.0 || y_var == 0.0){
    return 0.0;
  }

  return covariance / (sqrt(x_var) * sqrt(y_var));
}


double Calibration::estimate_volatility(PnlVect *x) {
  double biais = 0.0;
  double mean = 0.0;
  int n = x->size;
  double step = 1.0 /365.0;

  for (int i = 1 ; i < n; ++i){
    biais += pow(log(GET(x,i) / GET(x,i-1)) / sqrt(step), 2);
    mean += log(GET(x,i) / GET(x,i-1)) / sqrt(step);
  }

  return sqrt(biais / n - pow(mean / n , 2));
}


PnlVect* Calibration::volatilities(PnlMat *correlations) {
  PnlVect *sigma = pnl_vect_create(5);

  LET(sigma, 0) = sigma_1;
  LET(sigma, 1) = sqrt(pow(sigma_2,2)+pow(sigma_x1,2)+2*MGET(correlations, 1, 3)*sigma_2 * sigma_x1);
  LET(sigma, 2) = sqrt(pow(sigma_3,2)+pow(sigma_x2,2)+2*MGET(correlations, 2, 4)*sigma_3 * sigma_x2);
  LET(sigma, 3) = sigma_x1;
  LET(sigma, 4) = sigma_x2;

  return sigma;
}

PnlMat* Calibration::correlations(){
  PnlMat *correlations = pnl_mat_create(5,5);
  for (int i=0; i<5; i++){
    MLET(correlations, i, i) = 1.0;
  }
  MLET(correlations, 0, 1) = rho_12;
  MLET(correlations, 1, 0) = rho_12;
  MLET(correlations, 0, 2) = rho_13;
  MLET(correlations, 2, 0) = rho_13;
  MLET(correlations, 0, 3) = rho_1x1;
  MLET(correlations, 3, 0) = rho_1x1;
  MLET(correlations, 1, 2) = rho_23;
  MLET(correlations, 2, 1) = rho_23;
  MLET(correlations, 1, 3) = rho_2x1;
  MLET(correlations, 3, 1) = rho_2x1;
  MLET(correlations, 2, 3) = rho_3x1;
  MLET(correlations, 3, 2) = rho_3x1;
  MLET(correlations, 0, 4) = rho_1x2;
  MLET(correlations, 4, 0) = rho_1x2;
  MLET(correlations, 1, 4) = rho_2x2;
  MLET(correlations, 4, 1) = rho_2x2;
  MLET(correlations, 2, 4) = rho_3x2;
  MLET(correlations, 4, 2) = rho_3x2;
  MLET(correlations, 3, 4) = rho_x1x2;
  MLET(correlations, 4, 3) = rho_x1x2;

  return correlations;
}
