#include <string>
#include <istream>
#include <cmath>

#include "quote.hpp"
#include "time_utils.hpp"
#include "gtest/gtest.h"

using namespace std;

class QuoteTest : public ::testing::Test {
    protected:

        // Tolerance for double equality test
        double abs_error = 10e-5;

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
    EXPECT_NEAR(3527.550049, spot.getOpen(), this->abs_error);
    EXPECT_NEAR(3527.550049, spot.getHigh(), this->abs_error);
    EXPECT_NEAR(3527.550049, spot.getLow(), this->abs_error);
    EXPECT_NEAR(3527.550049, spot.getClose(), this->abs_error);

    // Test the spot at 2017-12-04
    spot = quote->getSpot("2017-12-04");
    EXPECT_NEAR(3576.219971, spot.getOpen(), this->abs_error);
    EXPECT_NEAR(3576.219971, spot.getHigh(), this->abs_error);
    EXPECT_NEAR(3576.219971, spot.getLow(), this->abs_error);
    EXPECT_NEAR(3576.219971, spot.getClose(), this->abs_error);
}

TEST_F(QuoteTest, EurUsd) {
    // EUR/USD rate
    Quote *quote = new Quote("EURUSD=X");
    quote->getHistoricalSpots("2017-12-01", "2017-12-31", "1d");

    // Test the spot at 2017-12-08
    Spot spot = quote->getSpot("2017-12-08");
    EXPECT_NEAR(1.17744, spot.getOpen(), this->abs_error);
    EXPECT_NEAR(1.17772, spot.getHigh(), this->abs_error);
    EXPECT_NEAR(1.17319, spot.getLow(), this->abs_error);
    EXPECT_NEAR(1.17722, spot.getClose(), this->abs_error);

    // Test the spot at 2017-12-21
    spot = quote->getSpot("2017-12-21");
    EXPECT_NEAR(1.18778, spot.getOpen(), this->abs_error);
    EXPECT_NEAR(1.1892, spot.getHigh(), this->abs_error);
    EXPECT_NEAR(1.18502, spot.getLow(), this->abs_error);
    EXPECT_NEAR(1.18782, spot.getClose(), this->abs_error);
}

TEST_F(QuoteTest, EurAud) {
    // EUR/AUD rate
    Quote *quote = new Quote("EURAUD=X");
    quote->getHistoricalSpots("2017-12-01", "2017-12-31", "1d");

    // Test the spot at 2017-12-13
    Spot spot = quote->getSpot("2017-12-13");
    EXPECT_NEAR(1.553756, spot.getOpen(), this->abs_error);
    EXPECT_NEAR(1.552764, spot.getHigh(), this->abs_error);
    EXPECT_NEAR(1.545433, spot.getLow(), this->abs_error);
    EXPECT_NEAR(1.553957, spot.getClose(), this->abs_error);

    // Test the spot at 2017-12-27
    spot = quote->getSpot("2017-12-27");
    EXPECT_NEAR(1.534314, spot.getOpen(), this->abs_error);
    EXPECT_NEAR(1.533646, spot.getHigh(), this->abs_error);
    EXPECT_NEAR(1.531682, spot.getLow(), this->abs_error);
    EXPECT_NEAR(1.533925, spot.getClose(), this->abs_error);
}
