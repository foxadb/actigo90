#include <cstdio>
#inlude "Data.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

using namespace std;
class Calibration {
private:
  double sigma_1, sigma_2, sigma_3, sigma_x1, sigma_x2;
  double rho_12, rho_13, rho_23, rho_1x1, rho_1x2, rho_2x1, rho_2x2, rho_3x1, rho_3x2, rho_x1x2;
  double estimate_volatility(PnlVect *x);
  double estimate_correlation(PnlVect *x, PnlVect *y);

public:
  const char *period1;
  const char *period2;

  Calibration(const char *period1, const char *period2);
  Calibration(Data *data);
  PnlVect* volatilities(PnlMat *correlations);
  PnlMat* correlations();
};
