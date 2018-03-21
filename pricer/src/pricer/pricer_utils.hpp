#ifndef PRICER_UTILS_H
#define PRICER_UTILS_H

#include "../quote/DataBaseManager.hpp"
#include "../quote/time_utils.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

#include <ctime>
#include <vector>

void vectorToPnlVect(std::vector<Spot> spots, PnlVect* spotsVect);

void getZeroCoupon(PnlVect *exchangeRate, double r, double maturity);

void getPastData(DataBaseManager *dbManager, PnlMat* past, std::vector<time_t> dates);

std::vector<time_t> getRightDates(time_t today, std::vector<time_t> dates);

#endif /* PRICER_UTILS_H */
