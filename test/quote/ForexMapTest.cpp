#include <string>
#include <cmath>
#include <ostream>
#include "pnl/pnl_vector.h"
#include "forex.hpp"
#include <map>
#include "curl_utils.hpp"
#include "time_utils.hpp"
#include "gtest/gtest.h"

class ForexMapTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            // code called before each test (beginning)
        }

        virtual void TearDown() {
            // code called before each test destructor (ending)
        }
};

TEST_F(ForexMapTest, getForexRatesInVect) {
  Forex *forex = new Forex("EUR", "USD");
  forex->getHistoricalRates("2017-10-03", "2017-11-05");
  std::time_t currentDate = dateToEpoch("2017-10-03");
  std::time_t endDate = dateToEpoch("2017-11-03");
  PnlVect *ratesVect = pnl_vect_create_from_scalar(31, 0.);
  int datesCounter = 0;
  std::map<std::string, double> rates = forex->getRates();

  while (currentDate < endDate){
    std::string current = epochToDate(currentDate);
    LET(ratesVect,datesCounter) = rates[current];
    std::cout << datesCounter << std::endl;
    datesCounter++;
    currentDate += 86400;
  }

  pnl_vect_print(ratesVect);     
}

    

