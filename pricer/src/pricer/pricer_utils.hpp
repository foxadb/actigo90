#ifndef PRICER_UTILS_H
#define PRICER_UTILS_H

#include "../quote/DataBaseManager.hpp"
#include "../quote/time_utils.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

#include <ctime>
#include <vector>
/**
* @brief fills the PnlVect with the vector
* @param spots
* @param spots
*/
void vectorToPnlVect(std::vector<Spot> spots, PnlVect* spots);
/**
* @brief fills the exchangeRate with calculated ZeroCoupon
* @param exchangeRate
* @param r
* @param maturity
*/
void getZeroCoupon(PnlVect *exchangeRate, double r, double maturity);
/**
* @brief fills "Past" matrix with the past data
* @param dbManager
* @param past
* @param dates
* @param rUsd
* @param rAud
*/
void getPastData(DataBaseManager *dbManager, PnlMat* past, std::vector<time_t> dates, double rUsd, double rAud);
/**
* @brief return a vector containing all the date from the start of the "semesterDates" to "todayDate"
* @param todayDate
* @param semesterDates
* @return  a vector of time_t
*/
std::vector<time_t> getRightDates(time_t todayDate, std::vector<time_t> semesterDates);
/**
* @brief return a vector containing all the date from the start of the "semesterDates" to "todayDate"
* @param dbm
* @param stock
* @param date
* @return double
*/
double getLastAvailableSpot(DataBaseManager *dbm, const char* stock, time_t date);
#endif /* PRICER_UTILS_H */
