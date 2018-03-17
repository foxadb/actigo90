#include "gtest/gtest.h"
#include "DataBaseManager.hpp"

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

TEST_F(DataBaseManagerTest, get_delta) {
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  double posted_delta = 0.02;
  dbManager->post_delta(posted_delta, "2018-01-01", "^GSPC");
  double returned_delta = dbManager->get_delta("2018-01-01", "^GSPC");

  EXPECT_EQ(posted_delta, returned_delta);
}
