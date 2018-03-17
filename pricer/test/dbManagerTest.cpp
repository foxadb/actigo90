#include "DataBaseManager.hpp"
#include <vector>
#include "spot.hpp"
#include <iostream>

int main(int argc, char **argv){
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  Spot spot = dbManager->getSpot("2017-01-19", "S&P 500");
  std::cout<<spot.toString()<<std::endl; 
  std::vector<Spot> spots = dbManager->getSpots("2017-01-19", "2017-01-20", "S&P 500");
  for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it)
    std::cout << ' ' << (*it).toString();
}
