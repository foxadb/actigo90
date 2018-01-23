#include "Calibration.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

double Calibration::estimate_correlation(PnlVect *x, PnlVect *y){

  double x_mean, y_mean = 0.0;
  int size = min(x->size, y->size);

  for (int i = 0; i < size; i++){
    x_mean += GET(x, i);
    y_mean += GET(y, i);
  }

  x_mean /= size,
  y_mean /= size;

  double covariance, x_var, y_var = 0.0;

  for (int i = 0; i < size; i++){
    covariance += (GET(x,i) - x_mean) * (GET(y,i) - y_mean);
    x_var += pow(GET(x,i) - x_mean, 2);
    y_var += pow(GET(y,i) - y_mean, 2);
  }

  if (x_var == 0.0 || y_var == 0.0){
    return 0.0;
  }

  return covariance / (sqrt(x_var) * sqrt(y_var));
}

double Calibration::estimate_volatility(string name){

  Quote* quote = new Quote(name);
  quote->getHistoricalSpots(period1, period2, interval);

  double biais = 0.0;
  double mean = 0.0;
  int n = quote->spots.size();

  for (int i = 1 ; i < n; i++){
    biais += pow(log(quote->spots[i].getClose() / quote->spots[i-1].getClose() / sqrt(step), 2);
    mean += log(quote->spots[i].getClose() / quote->spots[î-1].getClose()) / sqrt(step);
  }

  return sqrt(biais / n - pow(mean / n , 2));
}

PnlVect* Calibration::volatilities(PnlMat *correlations){
  PnlVect *sigma = pnl_vect_create(5);
  double r_euro = 0.1962	;
  double r_dolar = 0.02661;
  double r_australian_dolar = 0.02862;

  double sigma_1 = estimate_volatility("^STOXX50E");
  double sigma_2 = estimate_volatility("^GSPC");
  double sigma_3 = estimate_volatility("^AYQFN");
  double sigma_x1 = estimate_volatility()

  LET(sigma, 0) = sigma_1;



}
