#include <cmath>

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

TEST_F(ForexTest, EurUsd) {
     // Get the EUR/USD rate on 2018-01-01
    std::string symbols[] = { "USD" };
    double *rate = getForexRates("2018-01-02", "EUR", symbols, 1);

    ASSERT_EQ(1.2065, rate[0]);
}
