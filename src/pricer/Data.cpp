#include "Data.hpp"


Data::Data() {}

Data::Data(const Data& orig) {}

Data::~Data() {
    pnl_vect_free(&euroStoxSpots);
    pnl_vect_free(&spUsdSpots);
    pnl_vect_free(&spAudSpots);
    pnl_vect_free(&forexEurAud);
    pnl_vect_free(&forexEurUsd);
    pnl_mat_free(&dataMatrix);
}

Data::Data(PnlMat *matrixData){
   euroStoxSpots = pnl_vect_new();
   spUsdSpots = pnl_vect_new();
   spAudSpots = pnl_vect_new();
   forexEurUsd = pnl_vect_new();
   forexEurAud = pnl_vect_new();
   pnl_mat_get_col(euroStoxSpots, matrixData, 0);
   pnl_mat_get_col(spUsdSpots, matrixData, 1);
   pnl_mat_get_col(spAudSpots, matrixData, 2);
   pnl_mat_get_col(forexEurUsd, matrixData, 3);
   pnl_mat_get_col(forexEurAud, matrixData, 4);
}

Data::Data(const char* startDate, const char* currentDate){
  Quote* quote1 = new Quote("^STOXX50E");
  quote1->getHistoricalSpots(startDate, currentDate, "1d");
  euroStoxSpots = pnl_vect_new();
  pnl_vect_clone(euroStoxSpots, quote1->getCloseSpots());

  // spots for S&P 500
  Quote* quote2 = new Quote("^GSPC");
  quote2->getHistoricalSpots(startDate, currentDate, "1d");
  spUsdSpots = pnl_vect_new();
  pnl_vect_clone(spUsdSpots, quote2->getCloseSpots());

  // spots for S&P ASX 200
  Quote* quote3 = new Quote("^AXJO");
  quote3->getHistoricalSpots(startDate, currentDate, "1d");
  spAudSpots = pnl_vect_new();
  pnl_vect_clone(spAudSpots, quote3->getCloseSpots());

  // rates for USD/EUR
  Forex *forex1 = new Forex("USD", "EUR");
  forex1->getHistoricalSpots(startDate, currentDate);
  forexEurUsd = pnl_vect_new();
  pnl_vect_clone(forexEurUsd, forex1->getCloseSpots());

  // rates for AUD/EUR
  Forex *forex2 = new Forex("AUD", "EUR");
  forex2->getHistoricalSpots(startDate, currentDate);
  forexEurAud = pnl_vect_new();
  pnl_vect_clone(forexEurAud, forex2->getCloseSpots());

  //put spots vectors in dataMatrix
  dataMatrix = pnl_mat_create(euroStoxSpots->size, 5);
  pnl_mat_set_col(historicalDataMatrix, euroStoxSpots, 0);
  pnl_mat_set_col(historicalDataMatrix, spUsdSpots, 1);
  pnl_mat_set_col(historicalDataMatrix, spAudSpots, 2);
  pnl_mat_set_col(historicalDataMatrix, forexEurUsd, 3);
  pnl_mat_set_col(historicalDataMatrix, forexEurAud, 4);

  delete quote1, quote2, quote3, forex1, forex2;
}

void Data::getInitialSpots(PnlVect *initialSpots){
  pnl_mat_get_row(historicalDataMatrix, initialSpots, 0);
}

void getTodaySpots(PnlVect *todaySpots){
  pnl_mat_get_row(historicalDataMatrix, todaySpots, historicalDataMatrix->m -1);
}

void Data::completeData(int remainingDates, Option *option, PnlVect *currentSpots, PnlVect *volatilities, PnlMat *correlations){
}
