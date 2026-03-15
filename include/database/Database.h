#pragma once
#include <string>
#include <mongocxx/v_noabi/mongocxx/client.hpp>
#include <mongocxx/v_noabi/mongocxx/instance.hpp>
#include <mongocxx/v_noabi/mongocxx/database.hpp>
#include <bsoncxx/v_noabi/bsoncxx/document/value.hpp>

class DatabaseClient{
private:
        static DatabaseClient* instance;
        mongocxx::v_noabi::instance mongoInstance;
        mongocxx::v_noabi::client client;
        mongocxx::v_noabi::database db;

        DatabaseClient(const std:: string& uri, const std::string& dbName);

public:
        static DatabaseClient& getInstance(const std::string& uri="mongodb://localhost:27017", const std::string& dbName="fifa_simulator");
        void insertPlayer(const bsoncxx::v_noabi::document::value& doc);
        bool playerExists(std::string& playerName);
        long getPlayerCount();

        DatabaseClient(const DatabaseClient&) = delete;
        DatabaseClient& operator=(const DatabaseClient&) = delete;
};