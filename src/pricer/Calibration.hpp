#include <cstdio>
#include "pnl/pnl_vector.h"

using namespace std;
class Calibration {
public:
    double corr(PnlVect *x, PnlVect *y);
    double volatility(PnlVect *spots, PnlVect *dates);
};
