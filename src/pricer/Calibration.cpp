#include "Calibration.hpp"
#include <math.h>

double Calibration::corr(vector<double> x, vector<double> y){

  double x_mean, y_mean = 0.0;

  for (int i = 0; i < x.size(); i++){
    x_mean+= x[i];
    y_mean += y[i];
  }

  x_mean /= x.size();
  y_mean /= y.size();

  double covariance, x_var, y_var = 0.0;

  for (int i = 0; i < x.size(); i++){
    covariance += (x[i] - x_mean) * (y[i] - y_mean);
    x_var += pow(x[i] - x_mean, 2);
    y_var += pow(y[i] - y_mean, 2);
  }

  if (x_var == 0.0 || y_var == 0.0){
    return 0.0;
  }
  
  return covariance / (sqrt(x_var) * sqrt(y_var));
}

double Calibration::volatility(vector<double> index){

}
