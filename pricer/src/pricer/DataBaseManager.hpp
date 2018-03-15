#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include "pnl/pnl_vector.h"
#include <bsoncxx/types.hpp>

using namespace bsoncxx::types;


/**
 * @brief DataBaseManager class
 */

 class DataBaseManager{
 private:
   static DataBaseManager *dbManager;
   b_date getDate(const std::string& date);
   b_oid get_stock_id(const char* name);
   DataBaseManager();

 public:
   mongocxx::instance instance{};
   mongocxx::client client{mongocxx::uri{}};
   mongocxx::database db = client["peps"];
   static DataBaseManager* getDbManager();
   double getSpot(const std::string& date, const char* name);
 };

 #endif
