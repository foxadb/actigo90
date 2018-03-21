#include "gtest/gtest.h"
#include "DataBaseManager.hpp"
#include "spot.hpp"

using namespace std;

class DataBaseManagerTest : public ::testing::Test {
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

TEST_F(DataBaseManagerTest, getDelta) {
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  double posted_delta = 0.02;
  dbManager->postDelta(posted_delta, 1514764800, "^GSPC");
  double returned_delta = dbManager->getDelta(1514764800, "^GSPC");
  dbManager->clearDeltas();
  EXPECT_EQ(posted_delta, returned_delta);
}

TEST_F(DataBaseManagerTest, getSpot) {
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  Spot spot = dbManager->getSpot(1483401600, "^GSPC");
  EXPECT_EQ(spot.getClose(), 2257.830078);
}
