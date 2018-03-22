#include <string>
#include <istream>
#include <cmath>

#include "time_utils.hpp"
#include "gtest/gtest.h"

using namespace std;

class TimeUtilsTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            // code called before each test (beginning)
        }

        virtual void TearDown() {
            // code called before each test destructor (ending)
        }
};

TEST_F(TimeUtilsTest, dateToEpoch) {
    EXPECT_EQ(1428451200, dateToEpoch("2015-04-08"));
    EXPECT_EQ(1441929600, dateToEpoch("2015-09-11"));
    EXPECT_EQ(1468627200, dateToEpoch("2016-07-16"));
    EXPECT_EQ(1483142400, dateToEpoch("2016-12-31"));
    EXPECT_EQ(1520380800, dateToEpoch("2018-03-07"));
    EXPECT_EQ(1681516800, dateToEpoch("2023-04-15"));
}

TEST_F(TimeUtilsTest, epochToDate) {
    EXPECT_EQ("2015-04-15", epochToDate(1429056000));
    EXPECT_EQ("2016-02-29", epochToDate(1456704000));
    EXPECT_EQ("2017-01-01", epochToDate(1483228800));
    EXPECT_EQ("2017-03-03", epochToDate(1488499200));
    EXPECT_EQ("2017-06-21", epochToDate(1498003200));
    EXPECT_EQ("2022-11-07", epochToDate(1667779200));
}
