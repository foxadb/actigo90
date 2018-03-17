#include "DataBaseManager.hpp"
#include <cstdio>
#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/options/find.hpp>
#include "time_utils.hpp"

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


Spot DataBaseManager::getSpot(const char* date, const char* stock){
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

vector<Spot> DataBaseManager::getSpots(const char *start_date, const char* end_date, const char* stock){
  mongocxx::collection coll = db["spots"];
  vector<Spot> *spots = new vector<Spot>();
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
    std::string date = bDateToDate(date_ele.get_date());
    Spot *spot = new Spot(date, double(price_ele.get_double()));
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

double DataBaseManager::get_delta(const char* date, const char* stock){
  b_oid id = get_stock_id(stock);
  b_date bdate = read_date(date,1);
  mongocxx::collection coll = db["deltas"];
  double delta = 1.0;

  auto cursor = coll.find(make_document(kvp("stock", id), kvp("date", bdate)));
  for (const bsoncxx::document::view& doc : cursor) {
    bsoncxx::document::element id_ele = doc["delta"];
    if (id_ele.type() == type::k_double){
      delta = double(id_ele.get_double());
    }
  }

  return delta;
}
