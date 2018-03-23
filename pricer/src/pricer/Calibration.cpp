#include "Calibration.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

Calibration::Calibration(){
    step = 1.0 / 365.0;
    trends = pnl_vect_new();
    volatilities = pnl_vect_new();
    correlations = pnl_mat_new();
}


Calibration::Calibration(Data *data, double step){
    this->step = step;

    // Correlations estimation
    correlations = pnl_mat_create(5, 5);
    for (int i = 0; i < 5; i++){
        MLET(correlations, i, i) = 1.0;
    }

    double rho = estimate_correlation(data->euroStoxSpots, data->spUsdSpots);
    MLET(correlations, 0, 1) = rho;
    MLET(correlations, 1, 0) = rho;
    rho = estimate_correlation(data->euroStoxSpots, data->spAudSpots);
    MLET(correlations, 0, 2) = rho;
    MLET(correlations, 2, 0) = rho;
    rho = estimate_correlation(data->spUsdSpots, data->spAudSpots);
    MLET(correlations, 1, 2) = rho;
    MLET(correlations, 2, 1) = rho;
    rho = estimate_correlation(data->euroStoxSpots, data->eurUsd);
    MLET(correlations, 0, 3) = rho;
    MLET(correlations, 3, 0) = rho;
    rho = estimate_correlation(data->euroStoxSpots, data->eurAud);
    MLET(correlations, 0, 4) = rho;
    MLET(correlations, 4, 0) = rho;
    rho = estimate_correlation(data->spUsdSpots, data->eurUsd);
    MLET(correlations, 1, 3) = rho;
    MLET(correlations, 3, 1) = rho;
    rho = estimate_correlation(data->spUsdSpots, data->eurAud);
    MLET(correlations, 1, 4) = rho;
    MLET(correlations, 4, 1) = rho;
    rho = estimate_correlation(data->spAudSpots, data->eurUsd);
    MLET(correlations, 2, 3) = rho;
    MLET(correlations, 3, 2) = rho;
    rho = estimate_correlation(data->spAudSpots, data->eurAud);
    MLET(correlations, 2, 4) = rho;
    MLET(correlations, 4, 2) = rho;
    rho = estimate_correlation(data->eurUsd, data->eurAud);
    MLET(correlations, 3, 4) = rho;
    MLET(correlations, 4, 3) = rho;

    // Volatilities estimation
    PnlVect* tmpSpot = pnl_vect_create_from_scalar(data->euroStoxSpots->size, 0.);
    volatilities = pnl_vect_create(5);
    double sigma_1 = estimate_volatility(data->euroStoxSpots);
    pnl_mat_get_col(tmpSpot, data->historicalDataMatrix, 1);
    double sigma_2 = estimate_volatility(tmpSpot);
    pnl_mat_get_col(tmpSpot, data->historicalDataMatrix, 2);
    double sigma_3 = estimate_volatility(tmpSpot);
    pnl_mat_get_col(tmpSpot, data->historicalDataMatrix, 3);
    double sigma_x1 = estimate_volatility(tmpSpot);
    pnl_mat_get_col(tmpSpot, data->historicalDataMatrix, 4);
    double sigma_x2 = estimate_volatility(tmpSpot);

    PnlVect* tmpForex = pnl_vect_create_from_scalar(data->euroStoxSpots->size, 0.);
    pnl_mat_get_col(tmpSpot, data->historicalDataMatrix, 1);
    pnl_mat_get_col(tmpForex, data->historicalDataMatrix, 3);
    rho = estimate_correlation(tmpSpot, tmpForex);
    LET(volatilities, 0) = sigma_1;
    LET(volatilities, 1) = sqrt(pow(sigma_2,2)+pow(sigma_x1,2)+2*rho*sigma_2 * sigma_x1);
    pnl_mat_get_col(tmpSpot, data->historicalDataMatrix, 2);
    pnl_mat_get_col(tmpForex, data->historicalDataMatrix, 4);
    rho = estimate_correlation(tmpSpot, tmpForex);
    LET(volatilities, 2) = sqrt(pow(sigma_3,2)+pow(sigma_x2,2)+2*rho*sigma_3 * sigma_x2);
    LET(volatilities, 3) = sigma_x1;
    LET(volatilities, 4) = sigma_x2;

    // Trends estimation
    trends = pnl_vect_create(5);
    // We add sigma^2 / 2 each time to estimate the real trend. See estimate_trend() doc.
    LET(trends, 0) = estimate_trend(data->euroStoxSpots) + pow(sigma_1, 2) / 2.0;
    LET(trends, 1) = estimate_trend(data->spUsdSpots) + pow(sigma_2, 2) / 2.0;
    LET(trends, 2) = estimate_trend(data->spAudSpots) + pow(sigma_3, 2) / 2.0;
    LET(trends, 3) = estimate_trend(data->eurUsd) + pow(sigma_x1, 2) / 2.0;
    LET(trends, 4) = estimate_trend(data->eurAud) + pow(sigma_x2, 2) /2.0;

    // Free memory
    pnl_vect_free(&tmpSpot);
    pnl_vect_free(&tmpForex);
}


Calibration::~Calibration(){
    pnl_vect_free(&trends);
    pnl_vect_free(&volatilities);
    pnl_mat_free(&correlations);
}


double Calibration::estimate_correlation(PnlVect *x, PnlVect *y){

    int size = min(x->size, y->size);

    PnlVect *log_rent_x = pnl_vect_create(size -1);
    PnlVect *log_rent_y = pnl_vect_create(size -1);

    for (int i = 0; i < size - 1; i++){
        LET(log_rent_x, i) = log(GET(x, i+1) / GET(x, i));
        LET(log_rent_y, i) = log(GET(y, i+1) / GET(y, i));
    }

    double x_mean, y_mean = 0.0;

    for (int i = 0; i < size - 1; i++){
        x_mean += GET(log_rent_x, i);
        y_mean += GET(log_rent_y, i);
    }

    x_mean /= size - 1,
            y_mean /= size - 1;

    double covariance, x_var, y_var = 0.0;

    for (int i = 0; i < size - 1; i++){
        covariance += (GET(log_rent_x, i) - x_mean) * (GET(log_rent_y, i) - y_mean);
        x_var += pow(GET(log_rent_x, i) - x_mean, 2);
        y_var += pow(GET(log_rent_y, i) - y_mean, 2);
    }

    x_var /= size - 1;
    y_var /= size - 1;
    covariance /= size -1;

    covariance /= size - 1;
    x_var /= size - 1;
    y_var /= size - 1;
    pnl_vect_free(&log_rent_x);
    pnl_vect_free(&log_rent_y);

    if (x_var <= 0.0 || y_var <= 0.0){
        return 0.0;
    }

    return covariance / (sqrt(x_var) * sqrt(y_var));
}


double Calibration::estimate_volatility(PnlVect *x) {
    double biais = 0.0;
    double mean = 0.0;
    int n = x->size;
    for (int i = 1 ; i < n; ++i){
        biais += pow(log(GET(x, i) / GET(x, i - 1)) / sqrt(step), 2);
        mean += log(GET(x, i) / GET(x, i - 1)) / sqrt(step);
    }

    return sqrt(biais / (n - 1) - pow(mean / (n - 1) , 2));
}


double Calibration::estimate_trend(PnlVect *x){

    double mean = 0.0;

    for (int i = 1; i < x->size; i++){
        mean += log(GET(x, i) / GET(x, i - 1));
    }
    double meanPrime = log(GET(x, x->size - 1)/GET(x, 0));
    mean = mean / step ;
    mean = mean / (x->size - 1);
    return mean;
}


PnlVect* Calibration::getVolatilities() {
    return volatilities;
}


PnlMat* Calibration::getCorrelationsMatrix(){
    return correlations;
}


PnlVect* Calibration::getTrends(){
    return trends;
}
