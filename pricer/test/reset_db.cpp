#include "DataBaseManager.hpp"

int main(int argc, char** argv){
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  dbManager->clearDeltas();
  dbManager->clearPrices();
  return 0;
}
