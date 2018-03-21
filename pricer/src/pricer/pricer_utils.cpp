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


void getPastData(DataBaseManager *dbManager, PnlMat* past, std::vector<time_t> dates){
    int count = 0;
    PnlVect* euroStoxSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
    PnlVect* spUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
    PnlVect* spAudSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
    PnlVect* eurUsdSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
    PnlVect* eurAudSpots = pnl_vect_create_from_scalar(dates.size(), 0.);
    for (std::vector<time_t>::iterator it = dates.begin(); it != dates.end(); ++it){
        LET(euroStoxSpots, count) = dbManager->getSpot(*it, "^STOXX50E").getClose();
        LET(spUsdSpots, count) = dbManager->getSpot(*it, "^GSPC").getClose();
        LET(spAudSpots, count) = dbManager->getSpot(*it, "^AXJO").getClose();
        LET(eurUsdSpots, count) = dbManager->getSpot(*it, "EURUSD=X").getClose();
        LET(eurAudSpots, count) = dbManager->getSpot(*it, "EURAUD=X").getClose();
        count++;
    }
    pnl_vect_mult_vect_term (spUsdSpots, eurUsdSpots);
    pnl_vect_mult_vect_term (spAudSpots, eurAudSpots);

    //constructing zero coupons
    getZeroCoupon(eurUsdSpots, 0.05, 8.0);
    getZeroCoupon(eurAudSpots, 0.05, 8.0);

    pnl_mat_set_col(past, euroStoxSpots, 0);
    pnl_mat_set_col(past, spUsdSpots, 1);
    pnl_mat_set_col(past, spAudSpots, 2);
    pnl_mat_set_col(past, eurUsdSpots, 3);
    pnl_mat_set_col(past, eurAudSpots, 4);
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
