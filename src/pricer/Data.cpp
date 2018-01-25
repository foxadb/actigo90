#include "Data.hpp"

Data::Data() {}

Data::Data(const Data& orig) {}

Data::~Data() {
    pnl_vect_free(&euroStoxSpots);
    pnl_vect_free(&spUsdSpots);
    pnl_vect_free(&spAudSpots);
    pnl_vect_free(&eurAud);
    pnl_vect_free(&eurUsd);
    pnl_mat_free(&dataMatrix);
}

Data::Data(PnlMat *matrixData){
   euroStoxSpots = pnl_vect_new();
   spUsdSpots = pnl_vect_new();
   spAudSpots = pnl_vect_new();
   eurUsd = pnl_vect_new();
   eurAud = pnl_vect_new();
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
  Quote *quote4 = new Quote("EURUSD=X");
  quote4->getHistoricalSpots(startDate, currentDate, "1d");
  eurUsd = pnl_vect_new();
  pnl_vect_clone(eurUsd, quote4->getCloseSpots());

  // rates for AUD/EUR
  Quote *quote5 = new Quote("EURAUD=X");
  quote5->getHistoricalSpots(startDate, currentDate, "1d");
  eurAud = pnl_vect_new();
  pnl_vect_clone(eurAud, quote5->getCloseSpots());

  delete quote1, quote2, quote3, quote4, quote5;
}

void Data::completeData(int remainingDates, Option *option, PnlVect *currentSpots, PnlVect *volatilities, PnlMat *correlations){
}
