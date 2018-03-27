#include "pricer_utils.hpp"
#include <iostream>
#include <cmath>

void vectorToPnlVect(std::vector<Spot> spots, PnlVect* spotsVect){
    int count = 0;
    for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it){
        LET(spotsVect, count) = (*it).getClose();
        count++;
    }
}

void getZeroCoupon(PnlVect *exchangeRate, double r, double maturity){
    double step = maturity / exchangeRate->size;
    for (int i=0; i < exchangeRate->size; i++){
        LET(exchangeRate, i) *= std::exp(-r * (maturity - i * step));
    }
}


void getPastData(DataBaseManager *dbManager, PnlMat* past, std::vector<time_t> dates, double rUsd, double rAud){
    int count = 0;
    PnlVect* euroStoxSpots = pnl_vect_create_from_scalar(dates.size(), 0);
    PnlVect* spUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0);
    PnlVect* spAudSpots = pnl_vect_create_from_scalar(dates.size(), 0);
    PnlVect* eurUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0);
    PnlVect* eurAudSpots = pnl_vect_create_from_scalar(dates.size(), 0);

    for (std::vector<time_t>::iterator it = dates.begin(); it != dates.end(); ++it){
        LET(euroStoxSpots, count) = getLastAvailableSpot(dbManager, "^STOXX50E", *it);
        LET(spUsdSpots, count) = getLastAvailableSpot(dbManager, "^GSPC", *it);
        LET(spAudSpots, count) = getLastAvailableSpot(dbManager, "^AXJO", *it);
        LET(eurUsdSpots, count) = getLastAvailableSpot(dbManager, "EURUSD=X", *it);
        LET(eurAudSpots, count) = getLastAvailableSpot(dbManager, "EURAUD=X", *it);
        count++;
    }
    pnl_vect_mult_vect_term (spUsdSpots, eurUsdSpots);
    pnl_vect_mult_vect_term (spAudSpots, eurAudSpots);

    // Building zero coupons
    getZeroCoupon(eurUsdSpots, rUsd, 8.0);
    getZeroCoupon(eurAudSpots, rAud, 8.0);

    pnl_mat_set_col(past, euroStoxSpots, 0);
    pnl_mat_set_col(past, spUsdSpots, 1);
    pnl_mat_set_col(past, spAudSpots, 2);
    pnl_mat_set_col(past, eurUsdSpots, 3);
    pnl_mat_set_col(past, eurAudSpots, 4);

    // Free memory
    pnl_vect_free(&euroStoxSpots);
    pnl_vect_free(&spUsdSpots);
    pnl_vect_free(&spAudSpots);
    pnl_vect_free(&eurUsdSpots);
    pnl_vect_free(&eurAudSpots);
}

std::vector<time_t> getRightDates(time_t todayDate, std::vector<time_t> semesterDates){
    std::vector<time_t> rightDates;
    for (std::vector<time_t>::iterator it = (semesterDates).begin(); it != (semesterDates).end(); ++it){
        if ( (*it) >= todayDate)
            break;
        rightDates.push_back((*it));
    }
    rightDates.push_back(todayDate);
    return rightDates;
}

double getLastAvailableSpot(DataBaseManager *dbm, const char* stock, time_t date){
    double availablePrice = dbm->getSpot(date, stock).getClose();
    time_t oneDay = 86400;
    time_t currDate = date;
    while (availablePrice == 0){
      currDate = currDate - oneDay;
      availablePrice = dbm->getSpot(currDate, stock).getClose();
    }
    return availablePrice;
}
