#include "../quote/spot.hpp"
#include <ctime>
#include <vector>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../quote/DataBaseManager.hpp"
#include "../quote/time_utils.hpp"


void vectorToPnlVect(std::vector<Spot> spots, PnlVect* spotsVect);

void getZeroCoupon(PnlVect *exchangeRate, double r, double maturity);

void getPastData(DataBaseManager *dbManager, PnlMat* past, std::vector<time_t> dates);

std::vector<time_t> getRightDates(time_t today, std::vector<time_t> dates);
