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

TEST_F(CalibrationTest, correlation) {
  Calibration *calibration = new Calibration();
  PnlVect *x = pnl_vect_create(5);
  LET(x, 0) = 0.115;
  LET(x, 1) = 0.25;
  LET(x, 2) = 1.5614;
  LET(x, 3) = 2.541;
  LET(x, 4) = 0.521;
  double rho = calibration->estimate_correlation(x,x);
  EXPECT_TRUE(rho>0.99);
  EXPECT_TRUE(rho<1.01);
  pnl_vect_free(&x);
  delete calibration;
}

TEST_F(CalibrationTest, volatility) {
    PnlVect *x = pnl_vect_create_from_file("../../market-data/simul_call.dat");
    Calibration *calibration = new Calibration();
    double sigma = calibration->estimate_volatility(x);
    EXPECT_TRUE(sigma > 0.18);
    EXPECT_TRUE(sigma < 0.22);
    pnl_vect_free(&x);
    delete calibration;
 }
