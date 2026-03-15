#pragma once
#include <vector>
#include <memory>
#include "models/Player.h"
#include "api/APIClient.h"
#include "database/Database.h"

class PlayerFetcher
{
private:
    APIClient& apiClient;
    DatabaseClient& databaseClient;

public:
    PlayerFetcher(APIClient& client, DatabaseClient& db);
    int fetchAndCachePlayers(int limit=100);
private:
    bsoncxx::document::value playerToDocument(const nlohmann::json& data);
};