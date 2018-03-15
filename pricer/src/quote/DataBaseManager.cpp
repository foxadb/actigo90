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

b_oid DataBaseManager::get_stock_id(const char* stock){
  mongocxx::collection coll = db["stocks"];
  auto cursor = coll.find(make_document(kvp("name", stock)));
  for (const bsoncxx::document::view& doc : cursor) {
       bsoncxx::document::element id_ele = doc["_id"];
       return id_ele.get_oid();
     }
}

b_date DataBaseManager::read_date(const std::string& date, std::int32_t offset_from_utc){
  tm utc_tm{};
  istringstream ss{date};

  // Read time into std::tm.
  ss >> get_time(&utc_tm, "%Y-%m-%d");

  // Convert std::tm to std::time_t.
  time_t utc_time = mktime(&utc_tm);

  // Convert std::time_t std::chrono::systemclock::time_point.
  chrono::system_clock::time_point time_point = chrono::system_clock::from_time_t(utc_time);
  return b_date{time_point + chrono::hours{offset_from_utc}};
 }


Spot DataBaseManager::getSpot(const std::string& date, const char* stock){
  b_date bdate = read_date(date, 1);
  double price = getSpot(bdate, stock);
  Spot *spot = new Spot(date, price);
  return *spot;
}

double DataBaseManager::getSpot(b_date date, const char* stock){
  mongocxx::collection coll = db["spots"];
  b_oid id = get_stock_id(stock);
  double price = 0.0;
  auto cursor = coll.find(make_document(kvp("stock", id), kvp("date", date)));
  for (const bsoncxx::document::view& doc : cursor) {
    bsoncxx::document::element id_ele = doc["price"];
    if (id_ele.type() == type::k_double){
      price = double(id_ele.get_double());
    }
  }
  return price;
}

list<Spot> DataBaseManager::getSpots(const std::string& start_date, const std::string& end_date, const char* stock){
  mongocxx::collection coll = db["spots"];
  list<Spot> *spots = new list<Spot>();
  std::int32_t offset_from_utc = 1;
  b_oid id = get_stock_id(stock);
  bsoncxx::builder::basic::document filter;
  filter.append(kvp("stock", id), kvp("date", [start_date, end_date,
          offset_from_utc](bsoncxx::builder::basic::sub_document sd) {
          sd.append(kvp("$gte", read_date(start_date, offset_from_utc)));
          sd.append(kvp("$lte", read_date(end_date, offset_from_utc)));
      }));
  for (auto&& doc : coll.find(filter.view())) {
    bsoncxx::document::element price_ele = doc["price"];
    bsoncxx::document::element date_ele = doc["date"];
    chrono::system_clock::time_point time_point = chrono::system_clock::time_point(date_ele.get_date());
    time_t time = chrono::system_clock::to_time_t(time_point);
    Spot *spot = new Spot(ctime(&time), double(price_ele.get_double()));
    spots->push_back(*spot);
  }
  return *spots;
}

void DataBaseManager::post_delta(double delta, const char* date, const char* stock){
  b_oid id = get_stock_id(stock);
  b_date bdate = read_date(date, 1);
  
  bsoncxx::document::value doc = make_document(
        kvp("stock", id), kvp("date", bdate), kvp("delta", delta));

  auto res = db["deltas"].insert_one(std::move(doc));
}
