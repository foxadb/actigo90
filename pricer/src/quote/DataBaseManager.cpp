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

b_oid DataBaseManager::getStockId(const char* symbol){
    mongocxx::collection coll = db["stocks"];
    auto cursor = coll.find(make_document(kvp("symbol", symbol)));
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element id_ele = doc["_id"];
        return id_ele.get_oid();
    }
}

Spot DataBaseManager::getSpot(std::time_t date, const char* symbol){
    b_date bdate = read_date(date, 0);
    double price = getSpot(bdate, symbol);
    Spot spot = Spot(date, price);
    return spot;
}

double DataBaseManager::getSpot(b_date date, const char* symbol){
    mongocxx::collection coll = db["spots"];
    b_oid id = getStockId(symbol);
    double price = 0;

    auto cursor = coll.find(make_document(kvp("stock", id), kvp("date", date)));
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element id_ele = doc["price"];
        if (id_ele.type() == type::k_double){
            price = double(id_ele.get_double());
        }
    }

    return price;
}

vector<Spot> DataBaseManager::getSpots(std::time_t start_date, std::time_t end_date, const char* symbol){
    mongocxx::collection coll = db["spots"];
    vector<Spot> spots = vector<Spot>();
    std::int32_t offset_from_utc = 1;
    b_oid id = getStockId(symbol);
    bsoncxx::builder::basic::document filter;

    filter.append(kvp("stock", id), kvp("date", [start_date, end_date,
                                        offset_from_utc](bsoncxx::builder::basic::sub_document sd) {
        sd.append(kvp("$gte", read_date(start_date, offset_from_utc)));
        sd.append(kvp("$lte", read_date(end_date, offset_from_utc)));
    }));

    for (auto&& doc : coll.find(filter.view())) {
        bsoncxx::document::element price_ele = doc["price"];
        bsoncxx::document::element date_ele = doc["date"];
        if (date_ele.type() == type::k_date && price_ele.type() == type::k_double){
            b_date date = date_ele.get_date();
            double price = price_ele.get_double();
            Spot spot = Spot(bDateToEpoch(date), double(price));
            spots.push_back(spot);
        }
    }

    return spots;
}

void DataBaseManager::postDelta(double delta, std::time_t date, const char* symbol){
    b_oid id = getStockId(symbol);
    b_date bdate = read_date(date, 0);

    bsoncxx::document::value doc = make_document(
                kvp("stock", id), kvp("date", bdate), kvp("delta", delta));

    auto res = db["deltas"].insert_one(std::move(doc));
}

double DataBaseManager::getDelta(std::time_t date, const char* symbol){
    b_oid id = getStockId(symbol);
    b_date bdate = read_date(date, 0);
    mongocxx::collection coll = db["deltas"];
    double delta = 0;

    auto cursor = coll.find(make_document(kvp("stock", id), kvp("date", bdate)));
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element id_ele = doc["delta"];
        if (id_ele.type() == type::k_double){
            delta = double(id_ele.get_double());
        }
    }

    return delta;
}

void DataBaseManager::clearDeltas(){
    db["deltas"].delete_many({});
}

void DataBaseManager::postPrice(std::time_t date, double price, double portfolioValue){
    b_date bdate = read_date(date, 0);
    bsoncxx::document::value doc = make_document(
                kvp("date", bdate), kvp("actigo", price), kvp("hedging", portfolioValue));
    auto res = db["prices"].insert_one(std::move(doc));
}

double DataBaseManager::getActigoPrice(std::time_t date){
    b_date bdate = read_date(date, 0);
    mongocxx::collection coll = db["prices"];
    double actigo = 0;

    auto cursor = coll.find(make_document(kvp("date", bdate)));
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element id_ele = doc["actigo"];
        if (id_ele.type() == type::k_double){
            actigo = double(id_ele.get_double());
        }
    }

    return actigo;
}

double DataBaseManager::getHedging(std::time_t date) {
  b_date bdate = read_date(date, 0);
  mongocxx::collection coll = db["prices"];
  double hedging = 0.0;

  auto cursor = coll.find(make_document(kvp("date", bdate)));
  for (const bsoncxx::document::view& doc : cursor) {
    bsoncxx::document::element id_ele = doc["hedging"];
    if (id_ele.type() == type::k_double){
      hedging = double(id_ele.get_double());
    }
  }
  return hedging;
}

std::time_t DataBaseManager::getLastHedgingDate(){
  bsoncxx::builder::basic::document group_doc;
  mongocxx::collection coll = db["prices"];
  b_date date = read_date(0,0);
  mongocxx::pipeline stages;
  stages.group(make_document(
              kvp("_id", "last_date"),
              kvp("date", make_document(kvp("$max", "$date")))));
  auto cursor = coll.aggregate(stages);
  for (auto&& doc : cursor) {
    bsoncxx::document::element date_document = doc["date"];
    if (date_document.type() == type::k_date){
      date = date_document.get_date();
  }
  return bDateToEpoch(date);
  }
}

void DataBaseManager::clearPrices(){
    db["prices"].delete_many({});
}
