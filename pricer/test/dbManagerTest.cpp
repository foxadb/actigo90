#include "DataBaseManager.hpp"
#include "spot.hpp"

#include <vector>
#include <iostream>

int main(int argc, char **argv){
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  Spot spot = dbManager->getSpot(1484784000, "^GSPC");
  std::cout<<spot.toString()<<std::endl;

  std::vector<Spot> spots = dbManager->getSpots(1453161600, 1484784000, "^GSPC");
  for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it)
    std::cout << ' ' << (*it).toString()<<std::endl;
}
