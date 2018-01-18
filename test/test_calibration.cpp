#include <iostream>
#include "Calibration.hpp"
#include "parser.hpp"
#include "pnl/pnl_vector.h"

using namespace std;


int main(int argc, char** argv) {
  Calibration *cal = new Calibration();
  PnlVect *x = pnl_vect_create(5);
  PnlVect *y = pnl_vect_create(5);
  LET(x,0) = 0.1;
  LET(x,1) = 0.4;
  LET(x,2) = 1.2;
  LET(x,3) = 1.8;
  LET(x,4) = 2.0;
  LET(y,0) = 0.1;
  LET(y,1) = 0.7;
  LET(y,2) = 0.6;
  LET(y,3) = 1.1;
  LET(y,4) = 0.9;
  cout << cal->corr(x,y) << endl;

  PnlVect *spots = pnl_vect_create_from_file("../market-data/simul_call.dat");
  PnlVect *dates = pnl_vect_create(spots->size);
  LET(dates, 0) = 0.0;
  double prec = 0.0;
  for (int i = 1; i < dates->size; i++){
    LET(dates, i) = prec + 1.0/365.0;
    prec = GET(dates, i);
  }
  cout << cal->volatility(spots, dates) << endl;
  return 0;
}
