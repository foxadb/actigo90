// #include "Calibration.hpp"
// #include "gtest/gtest.h"
// #include "Data.hpp"

// using namespace std;

// class CalibrationTest : public ::testing::Test {
//     protected:

//         virtual void SetUp() {
//             // code called before each test (beginning)
//         }

//         virtual void TearDown() {
//             // code called before each test destructor (ending)
//         }
// };

// TEST_F(CalibrationTest, correlations) {
//   Data *data = new Data("2017-12-01", "2017-12-31");
//   Calibration *calibration = new Calibration(data);
//   PnlMat *correlations = calibration->correlations();
//   for (int i=0; i<5; i++){
//     for (int j=0; j<5; j++){
//       EXPECT_TRUE(MGET(correlations, i, j) >= 0.0);
//       EXPECT_TRUE(MGET(correlations, i, j) <= 1.0);
//     }
//   }
// }
