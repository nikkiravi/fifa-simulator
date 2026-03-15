#include "api/PlayerFetcher.h"
#include "utils/RatingGenerator.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <iostream>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

PlayerFetcher::PlayerFetcher(APIClient& client, DatabaseClient& db): apiClient(client), databaseClient(db){}

int PlayerFetcher::fetchAndCachePlayers(int limit){
    std::cout << "Fetching up to " << limit << " players from API...\n";

    auto teamsResponse = apiClient.get("/competitions/PL/teams");

    int cached = 0;
    for (const auto& team : teamsResponse["teams"]) {
        if (cached >= limit) break;

        int teamId = team["id"].get<int>();
        std::string clubName = team["name"].get<std::string>();

        auto teamResponse = apiClient.get("/teams/" + std::to_string(teamId));

        for (const auto& player : teamResponse["squad"]) {
            if (cached >= limit) break;

            std::string name = player["name"].get<std::string>();
            if (databaseClient.playerExists(name)) continue;

            std::string position = player.value("position", "Unknown");
            std::string nationality = player.value("nationality", "Unknown");

            nlohmann::json playerData;
            playerData["name"]        = name;
            playerData["position"]    = position;
            playerData["nationality"] = nationality;
            playerData["club"]        = clubName;
            playerData["rating"]      = RatingGenerator::generateRating(position);
            playerData["stats"]       = RatingGenerator::generateStats(position);

            auto doc = playerToDocument(playerData);
            databaseClient.insertPlayer(doc);
            cached++;

            std::cout << "Cached: " << name << " (" << clubName << ")\n";
        }
    }

    std::cout << "\nCached " << cached << " new players\n";
    std::cout << "Total players in DB: " << databaseClient.getPlayerCount() << "\n";
    return cached;
}

bsoncxx::document::value PlayerFetcher::playerToDocument(const nlohmann::json& data){
    auto stats = data["stats"];
    auto stats_doc = make_document(
        kvp("pace",      static_cast<int32_t>(stats["pace"].get<int>())),
        kvp("shooting",  static_cast<int32_t>(stats["shooting"].get<int>())),
        kvp("passing",   static_cast<int32_t>(stats["passing"].get<int>())),
        kvp("dribbling", static_cast<int32_t>(stats["dribbling"].get<int>())),
        kvp("defending", static_cast<int32_t>(stats["defending"].get<int>())),
        kvp("physical",  static_cast<int32_t>(stats["physical"].get<int>()))
    );

    return make_document(
        kvp("name",        data["name"].get<std::string>()),
        kvp("rating",      static_cast<int32_t>(data["rating"].get<int>())),
        kvp("position",    data["position"].get<std::string>()),
        kvp("nationality", data["nationality"].get<std::string>()),
        kvp("club",        data["club"].get<std::string>()),
        kvp("stats",       stats_doc)
    );
}