#include <cstdio>
#include "Data.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

using namespace std;
class Calibration {
private:
  PnlVect *volatilities;
  PnlMat *correlations;
  PnlVect *trends;


public:
  const char *period1;
  const char *period2;
  double estimate_volatility(PnlVect *x);
  double estimate_correlation(PnlVect *x, PnlVect *y);
  double estimate_trend(PnlVect *x);
  Calibration(const char *period1, const char *period2);
  Calibration(Data *data);
  PnlVect* getVolatilities();
  PnlMat* getCorrelationsMatrix();
  PnlVect* getTrends();
};
