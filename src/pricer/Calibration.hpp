#include <cstdio>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

using namespace std;
class Calibration {
private:
  double sigma_1;
  double sigma_2;
  double sigma_3;
  double sigma_x1;
  double sigma_x2;

public:
  const char *period1;
  const char *period2;

  Calibration(const char *period1, const char *period2);
  // double estimate_correlation(PnlVect *x, PnlVect *y);
  double estimate_volatility(PnlVect *x);
  PnlVect* volatilities(PnlMat *correlations);
  PnlMat* correlations();
};
