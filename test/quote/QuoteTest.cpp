#include <string>
#include <istream>
#include <cmath>

#include "quote.hpp"
#include "time_utils.hpp"
#include "gtest/gtest.h"

using namespace std;

class QuoteTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            // code called before each test (beginning)
        }

        virtual void TearDown() {
            // code called before each test destructor (ending)
        }
};

TEST_F(QuoteTest, getHistoricalCsv) {
    // S&P 500
    Quote *quote = new Quote("^GSPC");
    std::time_t epoch = dateToEpoch("2017-12-01") + 72000;
    std::string csv = quote->getHistoricalCsv(epoch, epoch, "1d");

    // Test the two CSV lines
    std::istringstream csvStream(csv);
    std::string line, expected;

    std::getline(csvStream, line);
    expected = "Date,Open,High,Low,Close,Adj Close,Volume";
    EXPECT_EQ(expected, line);

    std::getline(csvStream, line);
    expected = "2017-12-01,2645.100098,2650.620117,2605.520020,2642.219971,2642.219971,3942320000";
    EXPECT_EQ(expected, line);
}

TEST_F(QuoteTest, getHistoricalSpots) {
    // Euro Stoxx 50
    Quote *quote = new Quote("^STOXX50E");
    quote->getHistoricalSpots("2017-12-01", "2017-12-04", "1d");

    // Test the spot at 2017-12-01
    Spot spot = quote->getSpot("2017-12-01");
    EXPECT_EQ(3527.550049, spot.getOpen());
    EXPECT_EQ(3527.550049, spot.getHigh());
    EXPECT_EQ(3527.550049, spot.getLow());
    EXPECT_EQ(3527.550049, spot.getClose());

    spot = quote->getSpot("2017-12-04");
    EXPECT_EQ(3576.219971, spot.getOpen());
    EXPECT_EQ(3576.219971, spot.getHigh());
    EXPECT_EQ(3576.219971, spot.getLow());
    EXPECT_EQ(3576.219971, spot.getClose());
}
