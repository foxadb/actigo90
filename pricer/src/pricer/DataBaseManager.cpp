#include "DataBaseManager.hpp"
#include <cstdio>
#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/options/find.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>


using namespace std;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::type;

DataBaseManager* DataBaseManager::dbManager = 0;

DataBaseManager* DataBaseManager::getDbManager(){
  if (dbManager == 0){
      dbManager = new DataBaseManager();
  }
  return dbManager;
}

DataBaseManager::DataBaseManager(){}

b_oid DataBaseManager::get_stock_id(const char* name){
  mongocxx::collection stocksColl = db["stocks"];
  mongocxx::options::find opts{};
  opts.projection(make_document(kvp("_id", 1)));
  auto cursor = stocksColl.find(make_document(kvp("name", name)), opts);
  for (const bsoncxx::document::view& doc : cursor) {
       bsoncxx::document::element id_ele = doc["_id"];
       return id_ele.get_oid();
     }
}

b_date DataBaseManager::getDate(const std::string& date){
  tm utc_tm{};
  istringstream ss{date};

  // Read time into std::tm.
  ss >> get_time(&utc_tm, "%Y-%m-%d");

  // Convert std::tm to std::time_t.
  time_t utc_time = mktime(&utc_tm);

  // Convert std::time_t std::chrono::systemclock::time_point.
  chrono::system_clock::time_point time_point = chrono::system_clock::from_time_t(utc_time);

  return b_date{time_point + chrono::hours{1}};
 }

double DataBaseManager::getSpot(const std::string& date, const char* name){
  mongocxx::collection spotsColl = db["spots"];
  mongocxx::options::find opts{};
  opts.projection(make_document(kvp("price", 1)));
  b_oid id = get_stock_id(name);
  b_date bdate = getDate(date);
  auto cursor = spotsColl.find(make_document(kvp("stock", id), kvp("date", bdate)), opts);
         for (const bsoncxx::document::view& doc : cursor) {
           bsoncxx::document::element id_ele = doc["price"];
           return id_ele.get_double();
        }
}
