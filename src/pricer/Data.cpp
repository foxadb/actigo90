#include "data.hpp"

using namespace std;

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

Data::Data(const char* startDate, const char* currentDate){
  Quote* quote1 = new Quote("^STOXX50E");
  quote1->getHistoricalSpots(startDate, currentDate, "1d");
  euroStoxSpots = quote1->getCloseSpots();

  // spots for S&P 500
  Quote* quote2 = new Quote("^GSPC");
  quote2->getHistoricalSpots(startDate, currentDate, "1d");
  spUsdSpots = quote2->getCloseSpots();

  // spots for S&P ASX 200
  Quote* quote3 = new Quote("^AYQFN");
  quote3->getHistoricalSpots(startDate, currentDate, "1d");
  spAudSpots = quote3->getCloseSpots();

  // rates for USD/EUR
  Forex *forex1 = new Forex("USD", "EUR");
  forex1->getHistoricalSpots(startDate, currentDate);
  forexEurUsd = forex1->getCloseSpots();

  // rates for AUD/EUR
  Forex *forex2 = new Forex("AUD", "EUR");
  forex2->getHistoricalSpots(period1, period2);
  forexEurAud = forex2->getCloseSpots();

  delete quote1, quote2, quote3, forex1, forex2;
}

Data::completeData(int totalNumberOfdates){
  int remainingDates = totalNumberOfdates - euroStoxSpots->size;
  //simulate data in remainingDates and put it in dataMatrix;
}
