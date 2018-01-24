#include <cstdio>
#include "pnl/pnl_vector.h"

using namespace std;
class Calibration {
public:

  const char *period1;
  const char *period2;
  PnlVect *index1; //Euro_stoxx 50
  PnlVect *index2; //S&P 500
  PnlVect *index3; // S&P ASX 200
  PnlVect *x_1; // USD/Euro
  PnlVect *x_2; //AUD/EURO

  Calibration(const char *period1, const char *period2);
  // double estimate_correlation(PnlVect *x, PnlVect *y);
  double estimate_volatility(PnlVect *x);
  PnlVect* volatilities(PnlMat *correlations);
};
