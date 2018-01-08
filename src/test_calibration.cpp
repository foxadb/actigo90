#include <iostream>
#include "Calibration.hpp"

using namespace std;


int main(int argc, char** argv) {
  Calibration *cal = new Calibration();
  vector <double> x(5), y(5);
  x[0] = 0.1;
  x[1] = 0.4;
  x[2] = 1.2;
  x[3] = 1.8;
  x[4] = 2.0;
  y[0] = 0.1;
  y[1] = 0.7;
  y[2] = 0.6;
  y[3] = 1.1;
  y[4] = 0.9;
cout << cal->corr(x,y) << endl;
return 0;
}
