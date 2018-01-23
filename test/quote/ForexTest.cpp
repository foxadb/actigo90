#include <string>
#include <cmath>

#include "forex.hpp"
#include "curl_utils.hpp"
#include "gtest/gtest.h"

using namespace std;

class ForexTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            // code called before each test (beginning)
        }

        virtual void TearDown() {
            // code called before each test destructor (ending)
        }
};

TEST_F(ForexTest, getForexRates) {
     // Get the EUR/USD rate on 2018-01-01
    std::string symbols[] = { "USD" };
    double *rate = getForexRates("2018-01-02", "EUR", symbols, 1);

    // Test the rate
    EXPECT_EQ(1.2065, rate[0]);
}

TEST_F(ForexTest, getHistoricalRates) {
    // Get the EUR/AUD rates
    Forex *forex = new Forex("EUR", "AUD");
    forex->getHistoricalRates("2017-09-04", "2017-09-08");

    // Expected rates on the period
    double expectedRates[5] = { 1.496, 1.4902, 1.4961, 1.4926, 1.4881 };

    // Date iterator
    std::string date = "2017-09-0%";
    int dayNum = 4;

    for (int i = 0; i < 5; ++i) {
        // Replace the day number
        date.replace(9, 9, std::to_string(dayNum));

        // Test the rate
        EXPECT_EQ(expectedRates[i], forex->getRate(date));

        // Next day
        ++dayNum;
    }
}

TEST_F(ForexTest, getRate) {
    // Get the EUR/USD rate at 2017-01-22
    Forex *forex = new Forex("EUR", "USD");
    forex->getHistoricalRates("2018-01-22", "2018-01-22");

    // Test the rate
    EXPECT_EQ(1.2239, forex->getRate("2018-01-22"));
}
