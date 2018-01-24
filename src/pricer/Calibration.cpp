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

  // Historical spots for euro stoxx 50
  Quote* quote1 = new Quote("^STOXX50E");
  quote1->getHistoricalSpots(period1, period2, "1d");
  sigma_1 = estimate_volatility(quote1->getCloseSpots());
  delete quote1;

  // Hstorical spots for S&P 500
  Quote* quote2 = new Quote("^GSPC");
  quote2->getHistoricalSpots(period1, period2, "1d");
  sigma_2 = estimate_volatility(quote2->getCloseSpots());
  delete quote2;

  // Historical spots for S&P ASX 200
  Quote* quote3 = new Quote("^AYQFN");
  quote3->getHistoricalSpots(period1, period2, "1d");
  sigma_3 = estimate_volatility(quote3->getCloseSpots());
  delete quote3;

  // Historical spots for USD/EUR
  Forex *forex1 = new Forex("USD", "EUR");
  forex1->getHistoricalSpots(period1, period2);
  sigma_x1 = estimate_volatility(forex1->getCloseSpots());
  delete forex1;

  // Historical spots for AUD/EUR
  Forex *forex2 = new Forex("AUD", "EUR");
  forex2->getHistoricalSpots(period1, period2);
  sigma_x2 = estimate_volatility(forex2->getCloseSpots());
  delete forex2;

}


// double Calibration::estimate_correlation(PnlVect *x, PnlVect *y){
//
//   double x_mean, y_mean = 0.0;
//   int size = min(x->size, y->size);
//
//   for (int i = 0; i < size; i++){
//     x_mean += GET(x, i);
//     y_mean += GET(y, i);
//   }
//
//   x_mean /= size,
//   y_mean /= size;
//
//   double covariance, x_var, y_var = 0.0;
//
//   for (int i = 0; i < size; i++){
//     covariance += (GET(x,i) - x_mean) * (GET(y,i) - y_mean);
//     x_var += pow(GET(x,i) - x_mean, 2);
//     y_var += pow(GET(y,i) - y_mean, 2);
//   }
//
//   if (x_var == 0.0 || y_var == 0.0){
//     return 0.0;
//   }
//
//   return covariance / (sqrt(x_var) * sqrt(y_var));
// }


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
  LET(sigma, 1) = sqrt(pow(sigma_2,2)+pow(sigma_x1,2)+2*pnl_mat_get(correlations, 1, 3)*sigma_2 * sigma_x1);
  LET(sigma, 2) = sqrt(pow(sigma_3,2)+pow(sigma_x2,2)+2*pnl_mat_get(correlations, 2, 4)*sigma_3 * sigma_x2);
  LET(sigma, 3) = sigma_x1;
  LET(sigma, 4) = sigma_x2;

  return sigma;
}
