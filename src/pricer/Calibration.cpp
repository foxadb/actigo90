#include "Calibration.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

double Calibration::corr(PnlVect *x, PnlVect *y){

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

double Calibration::volatility(PnlVect *spots, PnlVect *dates){

  double biais = 0.0;
  double mean = 0.0;
  int n = spots->size;

  for (int i = 1 ; i < n; i++){
    biais += pow(log(GET(spots,i) / GET(spots,i-1)) / sqrt((GET(dates,i) - GET(dates,i-1))), 2);
    mean += log(GET(spots,i) / GET(spots,i-1)) / sqrt((GET(dates,i) - GET(dates,i-1)));
  }

  return sqrt(biais / n - pow(mean / n , 2));
}
