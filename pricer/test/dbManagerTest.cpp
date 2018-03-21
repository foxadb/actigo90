#include "DataBaseManager.hpp"
#include "spot.hpp"

#include <vector>
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Require epoch date argument" << std::endl;
    } else {
        DataBaseManager *dbManager = DataBaseManager::getDbManager();
        Spot spot = dbManager->getSpot(std::atoi(argv[1]), "^GSPC");
        //Spot spot = dbManager->getSpot(1484784000, "^GSPC");
        std::cout << spot.toString() << std::endl;

        /*
         * std::vector<Spot> spots = dbManager->getSpots(1453161600, 1484784000, "^GSPC");
    for (std::vector<Spot>::iterator it = spots.begin(); it != spots.end(); ++it)
        std::cout << ' ' << (*it).toString() << std::endl;
    dbManager->clearDeltas();
    */
    }

    return 0;
}
