#pragma once
#include <string>
#include <vector>
#include <mongocxx/v_noabi/mongocxx/client.hpp>
#include <mongocxx/v_noabi/mongocxx/instance.hpp>
#include <mongocxx/v_noabi/mongocxx/database.hpp>
#include <bsoncxx/v_noabi/bsoncxx/document/value.hpp>

// Plain struct holding all data for a player fetched from the DB
struct PlayerData {
    std::string name, nationality, club, position;
    int rating;
    int pace, shooting, passing, dribbling, defending, physical;
};

// Extends PlayerData with a rarity string for user_cards collection
struct UserCardData : PlayerData {
    std::string rarity;
};

// Holds all data for one saved squad (Milestone 5)
struct SquadData {
    std::string squadName;
    std::string formation;
    std::vector<UserCardData> players; // 11 entries indexed by slot; empty name = empty slot
};

class DatabaseClient{
private:
        static DatabaseClient* instance;
        mongocxx::v_noabi::instance mongoInstance;
        mongocxx::v_noabi::client client;
        mongocxx::v_noabi::database db;

        DatabaseClient(const std:: string& uri, const std::string& dbName);

public:
        static DatabaseClient& getInstance(const std::string& uri="mongodb://localhost:27017", const std::string& dbName="fifa_simulator");

        // Milestone 3 methods
        void insertPlayer(const bsoncxx::v_noabi::document::value& doc);
        bool playerExists(std::string& playerName);
        long getPlayerCount();

        // Milestone 4 methods - players collection
        std::vector<PlayerData> getPlayerDataByRatingRange(int minRating, int maxRating);

        // Milestone 4 methods - user_cards collection
        void insertUserCard(const bsoncxx::v_noabi::document::value& doc);
        std::vector<UserCardData> getUserCardData();
        long getUserCardCount();

        // Milestone 5 methods - squads collection
        void insertSquad(const bsoncxx::v_noabi::document::value& doc);
        SquadData getSquadByName(const std::string& squadName);
        std::vector<std::string> getAllSquadNames();

        DatabaseClient(const DatabaseClient&) = delete;
        DatabaseClient& operator=(const DatabaseClient&) = delete;
};