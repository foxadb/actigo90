#include "Data.hpp"
#include "BlackScholesModel.hpp"
#include <iostream>
using namespace std;

Data::Data() {}

Data::Data(const Data& orig) {}

Data::~Data() {
    pnl_vect_free(&euroStoxSpots);
    pnl_vect_free(&spUsdSpots);
    pnl_vect_free(&spAudSpots);
    pnl_vect_free(&eurAud);
    pnl_vect_free(&eurUsd);
    pnl_mat_free(&historicalDataMatrix);
    pnl_mat_free(&completeDataMatrix);
}

Data::Data(PnlMat *matrixData){
   euroStoxSpots = pnl_vect_new();
   spUsdSpots = pnl_vect_new();
   spAudSpots = pnl_vect_new();
   eurUsd = pnl_vect_new();
   eurAud = pnl_vect_new();
   historicalDataMatrix = pnl_mat_new();
   completeDataMatrix = pnl_mat_new();
   pnl_mat_get_col(euroStoxSpots, matrixData, 0);
   pnl_mat_get_col(spUsdSpots, matrixData, 1);
   pnl_mat_get_col(spAudSpots, matrixData, 2);
   pnl_mat_get_col(eurUsd, matrixData, 3);
   pnl_mat_get_col(eurAud, matrixData, 4);
}

Data::Data(const char* startDate, const char* currentDate){
  Quote *quote1 = new Quote("^STOXX50E");
  quote1->getHistoricalSpots(startDate, currentDate, "1d");
  euroStoxSpots = pnl_vect_new();
  pnl_vect_clone(euroStoxSpots, quote1->getCloseSpots());

  // spots for S&P 500
  Quote *quote2 = new Quote("^GSPC");
  quote2->getHistoricalSpots(startDate, currentDate, "1d");
  spUsdSpots = pnl_vect_new();
  pnl_vect_clone(spUsdSpots, quote2->getCloseSpots());

  // spots for S&P ASX 200
  Quote *quote3 = new Quote("^AXJO");
  quote3->getHistoricalSpots(startDate, currentDate, "1d");
  spAudSpots = pnl_vect_new();
  pnl_vect_clone(spAudSpots, quote3->getCloseSpots());

  // rates for USD/EUR
  Quote *quote4 = new Quote("USDEUR=X");
  quote4->getHistoricalSpots(startDate, currentDate, "1d");
  eurUsd = pnl_vect_new();
  pnl_vect_clone(eurUsd, quote4->getCloseSpots());

  // rates for AUD/EUR
  Quote *quote5 = new Quote("AUDEUR=X");
  quote5->getHistoricalSpots(startDate, currentDate, "1d");
  eurAud = pnl_vect_new();
  pnl_vect_clone(eurAud, quote5->getCloseSpots());

  //Recuperate the minimumSize of spots
  double minSize = MIN(euroStoxSpots->size, spUsdSpots->size);
  minSize = MIN(minSize, spAudSpots->size);
  minSize = MIN(minSize, eurUsd->size);
  minSize = MIN(minSize, eurAud->size);
  //resize spots
  pnl_vect_resize(euroStoxSpots, minSize);
  pnl_vect_resize(spUsdSpots, minSize);
  pnl_vect_resize(spAudSpots, minSize);
  pnl_vect_resize(eurUsd, minSize);
  pnl_vect_resize(eurAud, minSize);

  //put spots vectors in dataMatrix
  historicalDataMatrix = pnl_mat_create(euroStoxSpots->size, 5);
  pnl_mat_set_col(historicalDataMatrix, euroStoxSpots, 0);
  pnl_mat_set_col(historicalDataMatrix, spUsdSpots, 1);
  pnl_mat_set_col(historicalDataMatrix, spAudSpots, 2);
  pnl_mat_set_col(historicalDataMatrix, eurUsd, 3);
  pnl_mat_set_col(historicalDataMatrix, eurAud, 4);

  completeDataMatrix = pnl_mat_new();

  delete quote1, quote2, quote3, quote4, quote5;
}

void Data::getInitialSpots(PnlVect *initialSpots){
  pnl_mat_get_row(initialSpots, historicalDataMatrix, 0);
}

void Data::getTodaySpots(PnlVect *todaySpots){
  pnl_mat_get_row(todaySpots, historicalDataMatrix, historicalDataMatrix->m -1);
}

void Data::completeData(int remainingDates, Option *option, PnlVect *currentSpots, PnlVect *volatilities, PnlMat *correlations, PnlVect* trends, PnlRng *rng){
int totalSize = remainingDates + historicalDataMatrix->m;
BlackScholesModel *bs = new BlackScholesModel(option->size_, 0.04, correlations, volatilities, currentSpots);
bs->trend_ = trends;
double timeToMaturity = 8.0 - remainingDates/252; //to modify
PnlMat *remainingDataMatrix = pnl_mat_create(remainingDates+1, option->size_);
bs->simul_market(remainingDataMatrix, remainingDates+1, timeToMaturity, rng);
completeDataMatrix = pnl_mat_create_from_scalar(totalSize, option->size_, -50000);
pnl_mat_resize(historicalDataMatrix,historicalDataMatrix->m-1, historicalDataMatrix->n);
pnl_mat_set_subblock(completeDataMatrix, historicalDataMatrix, 0, 0);
pnl_mat_set_subblock(completeDataMatrix, remainingDataMatrix, historicalDataMatrix->m, 0);

}
