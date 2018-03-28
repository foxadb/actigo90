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

    /**
     * @brief Get the id coresponding to the symbol
     * @param char* symbol
     * @return b_oid id
     */
    b_oid getStockId(const char* symbol);

    /**
     * @brief Get the price
     * @param date date
     * @param char*symbol
     * @return double containing the price of the actif coresponding to symbol at the
     */
    double getSpot(b_date date, const char* symbol);
    /**
     * @brief Constructeur
     */
    DataBaseManager();

public:
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};
    mongocxx::database db = client["peps"];

    /**
     * @brief Get the dbManager
     * @return a pointer on the dbManager
     */
    static DataBaseManager* getDbManager();

    /**
     * @brief Get the price
     * @param date date
     * @param char* symbol
     * @return double containing the price of the actif coresponding to symbol at the
     */
    Spot getSpot(std::time_t date, const char* symbol);

    /**
     * @brief Get the spots of the period
     * @param startDate Begining date (POSIX timestamp)
     * @param endDate Ending date (POSIX timestamp)
     * @param char* symbol
     * @return  a vector containing all the spots of the actif corespondig to symbol of the duration
     */
    std::vector<Spot> getSpots(std::time_t startDate,std::time_t endDate, const char* symbol);

    /**
    * @brief POST the Delta in th DataBase
    * @param delta
    * @param date
    * @param char* symbol
    */
    void postDelta(double delta, std::time_t date, const char* symbol);

    /**
    * @brief Get the Delta of the acitf corespondig to symbol at the Date from the Data Base
    * @param delta
    * @param date
    * @param char* symbol
    * post the delta in the data base
    */
    double getDelta(std::time_t date, const char* symbol);

    /**
    * @brief fonction non implementé
    */
    double getPreviousDelta(std::time_t current, const char* symbol, double &previous_delta, std::time_t &previous_date);

    /**
    * @brief delete all the delta from the Data Base
    */
    void clearDeltas();

    /**
    * @brief POST the Price in the DataBase
    * @param date
    * @param price the price of actigo
    * @param portfolioValue the price of the hedgin
    */
    void postPrice(std::time_t date, double price, double portfolioValue);
};

#endif /* DATABASEMANAGER_H */
