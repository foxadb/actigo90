#include "Calibration.hpp"

double Calibration::corr(vector<double> index_1, vector<double> index_2){
  double corr;
  alglib::real_1d_array data_1, data_2;
  data_1.setcontent(index_1.size(), &(index_1[0]));
  data_2.setcontent(index_2.size(), &(index_2[0]));
  corr = alglib::spearmancorr2(data_1, data_2);
  return corr;
}

double Calibration::volatility(vector<double> index){

}
