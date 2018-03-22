#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/types.hpp>
#include "spot.hpp"
#include <vector>

using namespace bsoncxx::types;


/**
 * @brief DataBaseManager class
 */

class DataBaseManager{
private:
    static DataBaseManager *dbManager;
    b_oid getStockId(const char* symbol);
    double getSpot(b_date date, const char* symbol);
    DataBaseManager();

public:
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};
    mongocxx::database db = client["peps"];
    static DataBaseManager* getDbManager();
    Spot getSpot(std::time_t date, const char* symbol);
    std::vector<Spot> getSpots(std::time_t startDate,
                               std::time_t endDate, const char* symbol);
    void postDelta(double delta, std::time_t date, const char* symbol);
    double getDelta(std::time_t date, const char* symbol);
    double getPreviousDelta(std::time_t current, const char* symbol, double &previous_delta, std::time_t &previous_date);
    void clearDeltas();
    void postPrice(std::time_t date, double price, double portfolioValue);
};

#endif /* DATABASEMANAGER_H */
