#include "DataBaseManager.hpp"
#include <vector>
#include "spot.hpp"
#include <iostream>

int main(int argc, char **argv){
  DataBaseManager *dbManager = DataBaseManager::getDbManager();
  Spot spot = dbManager->getSpot("2017-01-19", "^GSPC");
  std::cout<<spot.toString()<<std::endl;

  std::vector<Spot> spots = dbManager->getSpots("2016-01-19", "2017-01-19", "^GSPC");
  for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it)
    std::cout << ' ' << (*it).toString()<<std::endl;

    std::vector<Spot> spots1 = dbManager->getSpots("2016-01-19", "2017-01-19", "^STOXX50E");
    for (std::vector<Spot>::iterator it = spots1.begin(); it != spots1.end(); ++it)
      std::cout << ' ' << (*it).toString()<<std::endl;

      std::vector<Spot> euroStoxSpotspots = dbManager->getSpots("2016-01-19", "2016-01-19", "^STOXX50E");
      //vectorToPnlVect(euroStoxSpotspots, euroStoxSpots);
      std::vector<Spot> spUsdSpotspots = dbManager->getSpots("2016-01-19", "2016-01-19", "^GSPC");
      //vectorToPnlVect(spUsdSpotspots, spUsdSpots);
      std::vector<Spot> spAudSpotspots = dbManager->getSpots("2016-01-19", "2016-01-19", "^AXJO");
      //vectorToPnlVect(spAudSpotspots, spAudSpots);
      std::vector<Spot> eurUsdSpotspots = dbManager->getSpots("2016-01-19", "2016-01-19", "EURUSD=X");
      //vectorToPnlVect(eurUsdSpotspots, eurUsdSpots);
      std::vector<Spot> eurAudSpotspots = dbManager->getSpots("2016-01-19", "2016-01-19", "EURAUD=X");

  /*dbManager->postDelta(0.05, "2018-01-19", "^GSPC");
  double delta = dbManager->getDelta("2018-01-19", "^GSPC");
  std::cout << delta << std::endl;*/
}
