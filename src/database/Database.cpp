#include "database/Database.h"
#include <bsoncxx/v_noabi/bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/v_noabi/bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/v_noabi/bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/v_noabi/bsoncxx/builder/basic/kvp.hpp>
#include <iostream>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;


DatabaseClient* DatabaseClient::instance = nullptr;
DatabaseClient& DatabaseClient::getInstance(const std::string& uri, const std::string& dbName){
    if (!instance){
        instance = new DatabaseClient(uri, dbName);
    }
    return *instance;
}

DatabaseClient::DatabaseClient(const std::string& uri, const std::string& dbName): client(mongocxx::uri{uri}), db(client[dbName]){
    std::cout << "Connected to MongoDB: " << dbName << std::endl;
}

void DatabaseClient::insertPlayer(const bsoncxx::v_noabi::document::value& doc){
    auto collection = db["players"];
    collection.insert_one(doc.view());
}

bool DatabaseClient::playerExists(std::string& playerName){
    auto collection = db["players"];
    auto query = make_document(kvp("name", playerName));
    auto result = collection.find_one(query.view());
    return result.has_value();
}

long DatabaseClient::getPlayerCount(){
    auto collection = db["players"];
    return collection.count_documents({});
}