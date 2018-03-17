#include "DataBaseManager.hpp"
#include <vector>
#include "spot.hpp"
#include <iostream>

int main(int argc, char **argv){
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  Spot spot = dbManager->getSpot("2017-01-19", "^GSPC");
  std::cout<<spot.toString()<<std::endl;

  std::vector<Spot> spots = dbManager->getSpots("2017-01-01", "2017-01-20", "^GSPC");
  for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it)
    std::cout << ' ' << (*it).toString()<<std::endl;

  dbManager->postDelta(0.05, "2018-01-19", "^GSPC");
  double delta = dbManager->getDelta("2018-01-19", "^GSPC");
  std::cout << delta << std::endl;
}
