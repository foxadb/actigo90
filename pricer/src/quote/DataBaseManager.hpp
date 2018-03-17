#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/types.hpp>
#include "spot.hpp"
#include <list>

using namespace bsoncxx::types;


/**
 * @brief DataBaseManager class
 */

 class DataBaseManager{
 private:
   static DataBaseManager *dbManager;
   static b_date read_date(const std::string& date,
     std::int32_t offset_from_utc);
   b_oid get_stock_id(const char* stock);
   double getSpot(b_date date, const char* stock);
   DataBaseManager();

 public:
   mongocxx::instance instance{};
   mongocxx::client client{mongocxx::uri{}};
   mongocxx::database db = client["peps"];
   static DataBaseManager* getDbManager();
   Spot getSpot(const std::string& date, const char* stock);
   std::list<Spot> getSpots(const std::string& startDate,
     const std::string& endDate, const char* stock);
   void post_delta(double delta, const char* date, const char* stock);

 };

 #endif
