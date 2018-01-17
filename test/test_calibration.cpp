#include <iostream>
#include "Calibration.hpp"
#include "parser.hpp"
#include "pnl/pnl_vector.h"

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
  cout << "test"<< endl;
//cout << cal->corr(x,y) << endl;

PnlVect *spots = pnl_vect_create_from_file("../market-data/simul_call.dat");
PnlVect *dates = pnl_vect_create(spots->size);
LET(dates, 0) = 0.0;
double prec = 0.0;
for (int i = 1; i < dates->size; i++){
    LET(dates, i) = prec + 1.0/365.0;
    prec = GET(dates, i);
}
pnl_vect_print(dates); 
cout << "spots"<< endl; 
pnl_vect_print(spots);
cout << cal->volatility(spots, dates) << endl;
return 0;
}