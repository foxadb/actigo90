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

  delete quote1, quote2, quote3, forex1, forex2;
}

void Data::completeData(int remainingDates, Option *option, PnlVect *currentSpots, PnlVect *volatilities, PnlMat *correlations){
}
