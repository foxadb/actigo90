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
    pnl_mat_free(&historicalDataMatrixEuro);
    pnl_mat_free(&completeDataMatrix);
}

Data::Data(PnlMat *matrixData){
   euroStoxSpots = pnl_vect_new();
   spUsdSpots = pnl_vect_new();
   spAudSpots = pnl_vect_new();
   eurUsd = pnl_vect_new();
   eurAud = pnl_vect_new();
   historicalDataMatrix = pnl_mat_new();
   historicalDataMatrixEuro = pnl_mat_new();
   completeDataMatrix = pnl_mat_new();
   historicalDataMatrix = matrixData;
   pnl_mat_get_col(euroStoxSpots, matrixData, 0);
   pnl_mat_get_col(spUsdSpots, matrixData, 1);
   pnl_mat_get_col(spAudSpots, matrixData, 2);
   pnl_mat_get_col(eurUsd, matrixData, 3);
   pnl_mat_get_col(eurAud, matrixData, 4);
   //converting everyting to euro
   pnl_vect_mult_vect_term (spUsdSpots, eurUsd);
   pnl_vect_mult_vect_term (spAudSpots, eurAud);

   //constructing zero coupons
   rEur = 0.05;
   rUsd = 0.05;
   rAud = 0.05;
   getZeroCoupon(eurUsd, rUsd, 8.0);
   getZeroCoupon(eurAud, rAud, 8.0);

}

Data::Data(const char* startDate, const char* currentDate){

  rEur = 0.05;
  rUsd = 0.05;
  rAud = 0.05;

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

    //converting everyting to euro
  pnl_vect_mult_vect_term (spUsdSpots, eurUsd);
  pnl_vect_mult_vect_term (spAudSpots, eurAud);

  //constructing zero coupons
  getZeroCoupon(eurUsd, rUsd, 8.0);
  getZeroCoupon(eurAud, rAud, 8.0);

  historicalDataMatrixEuro = pnl_mat_create(euroStoxSpots->size, 5);
  pnl_mat_set_col(historicalDataMatrixEuro, euroStoxSpots, 0);
  pnl_mat_set_col(historicalDataMatrixEuro, spUsdSpots, 1);
  pnl_mat_set_col(historicalDataMatrixEuro, spAudSpots, 2);
  pnl_mat_set_col(historicalDataMatrixEuro, eurUsd, 3);
  pnl_mat_set_col(historicalDataMatrixEuro, eurAud, 4);
  //pnl_mat_print(historicalDataMatrixEuro);


  completeDataMatrix = pnl_mat_new();

  delete quote1, quote2, quote3, quote4, quote5;
}

void Data::getInitialSpots(PnlVect *initialSpots){
  pnl_mat_get_row(initialSpots, historicalDataMatrix, 0);
}

void Data::getInitialSpotsEuro(PnlVect *initialSpotsEuro){
  pnl_mat_get_row(initialSpotsEuro, historicalDataMatrixEuro, 0);
}

void Data::getTodaySpots(PnlVect *todaySpots){
  pnl_mat_get_row(todaySpots, historicalDataMatrixEuro, historicalDataMatrix->m -1);
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
  pnl_mat_set_subblock(completeDataMatrix, historicalDataMatrixEuro, 0, 0);
  pnl_mat_set_subblock(completeDataMatrix, remainingDataMatrix, historicalDataMatrix->m, 0);
}

void Data::getDataAtRebalancingDates(PnlMat* path, int rebalancingFrequency){
  int totalNumberOfDates = historicalDataMatrix->m;
  int totalRebalancingDate = totalNumberOfDates / rebalancingFrequency;
  PnlVect *priceVect = pnl_vect_create_from_scalar(5,0);
  int rebalancingDate = 0;
  //borne à revoir dans des cas
  for (int date = 0; date < totalNumberOfDates - rebalancingFrequency; date += rebalancingFrequency){
    pnl_mat_get_row(priceVect, historicalDataMatrix, date);
    pnl_mat_set_row(path, priceVect, rebalancingDate);
    rebalancingDate++;
  }
  pnl_mat_print(path);
  pnl_vect_free(&priceVect);
}

void Data::getZeroCoupon(PnlVect *exchangeRate, double r, double maturity){
  double step = maturity / exchangeRate->size;
    for (int i=0; i < exchangeRate->size; i++){
      LET(exchangeRate, i) *= exp(-r * (maturity - i * step));
    }
}

/*void Data::vectorToPnlVect(vector<spot> spots, PnlVect* spotsVect){
  spotsVect = pnl_vect_create_from_scalar(spots.size(), 0.);
  int count = 0;
  for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it){
     MLET(spotsVect, count) = it.getClose();
  }
}*/
