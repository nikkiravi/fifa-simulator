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

std::vector<PlayerData> DatabaseClient::getPlayerDataByRatingRange(int minRating, int maxRating){
    auto collection = db["players"];
    auto query = make_document(
        kvp("rating", make_document(
            kvp("$gte", static_cast<int32_t>(minRating)),
            kvp("$lte", static_cast<int32_t>(maxRating))
        ))
    );

    std::vector<PlayerData> results;
    auto cursor = collection.find(query.view());
    for (auto&& doc : cursor) {
        PlayerData pd;
        pd.name        = std::string(doc["name"].get_string().value);
        pd.nationality = std::string(doc["nationality"].get_string().value);
        pd.club        = std::string(doc["club"].get_string().value);
        pd.position    = std::string(doc["position"].get_string().value);
        pd.rating      = doc["rating"].get_int32().value;

        auto stats     = doc["stats"].get_document().value;
        pd.pace        = stats["pace"].get_int32().value;
        pd.shooting    = stats["shooting"].get_int32().value;
        pd.passing     = stats["passing"].get_int32().value;
        pd.dribbling   = stats["dribbling"].get_int32().value;
        pd.defending   = stats["defending"].get_int32().value;
        pd.physical    = stats["physical"].get_int32().value;

        results.push_back(pd);
    }
    return results;
}

void DatabaseClient::insertUserCard(const bsoncxx::v_noabi::document::value& doc){
    auto collection = db["user_cards"];
    collection.insert_one(doc.view());
}

std::vector<UserCardData> DatabaseClient::getUserCardData(){
    auto collection = db["user_cards"];
    std::vector<UserCardData> results;
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        UserCardData ucd;
        ucd.name        = std::string(doc["name"].get_string().value);
        ucd.nationality = std::string(doc["nationality"].get_string().value);
        ucd.club        = std::string(doc["club"].get_string().value);
        ucd.position    = std::string(doc["position"].get_string().value);
        ucd.rating      = doc["rating"].get_int32().value;
        ucd.rarity      = std::string(doc["rarity"].get_string().value);

        auto stats      = doc["stats"].get_document().value;
        ucd.pace        = stats["pace"].get_int32().value;
        ucd.shooting    = stats["shooting"].get_int32().value;
        ucd.passing     = stats["passing"].get_int32().value;
        ucd.dribbling   = stats["dribbling"].get_int32().value;
        ucd.defending   = stats["defending"].get_int32().value;
        ucd.physical    = stats["physical"].get_int32().value;

        results.push_back(ucd);
    }
    return results;
}

long DatabaseClient::getUserCardCount(){
    auto collection = db["user_cards"];
    return collection.count_documents({});
}

// ── Milestone 5 – squads collection ─────────────────────────────────────────

void DatabaseClient::insertSquad(const bsoncxx::v_noabi::document::value& doc) {
    auto collection = db["squads"];
    collection.insert_one(doc.view());
}

SquadData DatabaseClient::getSquadByName(const std::string& squadName) {
    auto collection = db["squads"];
    auto query = make_document(kvp("name", squadName));
    auto result = collection.find_one(query.view());
    if (!result) {
        throw std::runtime_error("Squad not found: " + squadName);
    }

    SquadData sd;
    auto doc = result->view();
    sd.squadName = std::string(doc["name"].get_string().value);
    sd.formation = std::string(doc["formation"].get_string().value);
    sd.players.resize(11); // indexed by slot number

    for (auto&& playerDoc : doc["players"].get_array().value) {
        auto pdoc    = playerDoc.get_document().value;
        int slotIdx  = pdoc["slot"].get_int32().value;

        UserCardData ucd;
        ucd.name = std::string(pdoc["name"].get_string().value);
        if (!ucd.name.empty()) {
            ucd.nationality = std::string(pdoc["nationality"].get_string().value);
            ucd.club        = std::string(pdoc["club"].get_string().value);
            ucd.position    = std::string(pdoc["position"].get_string().value);
            ucd.rating      = pdoc["rating"].get_int32().value;
            ucd.rarity      = std::string(pdoc["rarity"].get_string().value);

            auto stats      = pdoc["stats"].get_document().value;
            ucd.pace        = stats["pace"].get_int32().value;
            ucd.shooting    = stats["shooting"].get_int32().value;
            ucd.passing     = stats["passing"].get_int32().value;
            ucd.dribbling   = stats["dribbling"].get_int32().value;
            ucd.defending   = stats["defending"].get_int32().value;
            ucd.physical    = stats["physical"].get_int32().value;
        }
        sd.players[slotIdx] = ucd;
    }
    return sd;
}

std::vector<std::string> DatabaseClient::getAllSquadNames() {
    auto collection = db["squads"];
    std::vector<std::string> names;
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        names.push_back(std::string(doc["name"].get_string().value));
    }
    return names;
}