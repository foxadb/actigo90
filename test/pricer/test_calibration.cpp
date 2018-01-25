#include "Calibration.hpp"
#include "gtest/gtest.h"
#include "Data.hpp"
#include "iostream"

using namespace std;

class CalibrationTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            // code called before each test (beginning)
        }

        virtual void TearDown() {
            // code called before each test destructor (ending)
        }
};

TEST_F(CalibrationTest, correlations) {
  Data *data = new Data("2017-12-01", "2017-12-15");
  Calibration *calibration = new Calibration(data);
  PnlMat *correlations = calibration->getCorrelationsMatrix();
  pnl_mat_print(correlations);
  for (int i=0; i<5; i++){
    for (int j=0; j<5; j++){
      EXPECT_TRUE(MGET(correlations, i, j) >= -1.0);
      EXPECT_TRUE(MGET(correlations, i, j) <= 1.0);
    }
  }
}
