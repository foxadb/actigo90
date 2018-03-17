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
   Spot getSpot(const char *date, const char* symbol);
   std::vector<Spot> getSpots(const char *startDate,
     const char *endDate, const char* symbol);
   void postDelta(double delta, const char* date, const char* symbol);
   double getDelta(const char* date, const char* symbol);

 };

 #endif
