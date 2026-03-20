// include/api/PlayerFetcher.h
#pragma once
#include <vector>
#include <memory>
#include "models/Player.h"
#include "api/APIClient.h"
#include "database/Database.h"
#include "utils/RatingGenerator.h"

/**
 * @brief Fetches real player data from the football-data.org API and caches it in MongoDB.
 *
 * PlayerFetcher coordinates three subsystems:
 *  1. **APIClient** — fetches competition teams and their squad rosters via HTTP GET.
 *  2. **RatingGenerator** — synthesises realistic FIFA-style stats for each player
 *     (the API does not provide player ratings directly).
 *  3. **DatabaseClient** — persists each fetched player to the "players" collection,
 *     skipping any player who already exists (idempotent).
 *
 * Example usage (run once to populate the database):
 * @code
 *   PlayerFetcher fetcher(APIClient::getInstance(), DatabaseClient::getInstance());
 *   int count = fetcher.fetchAndCachePlayers(100);
 *   std::cout << "Cached " << count << " new players.\n";
 * @endcode
 */
class PlayerFetcher {
private:
    APIClient& apiClient;         ///< Reference to the shared HTTP client.
    DatabaseClient& databaseClient; ///< Reference to the shared database client.

    /**
     * @brief Converts a player JSON object from the API into a BSON document.
     *
     * Generates FIFA-style statistics via RatingGenerator and constructs
     * the BSON document ready for insertion into MongoDB.
     *
     * @param data JSON object for one player from the football-data.org API.
     * @return BSON document value representing the player.
     */
    bsoncxx::document::value playerToDocument(const nlohmann::json& data);

public:
    /**
     * @brief Constructs a PlayerFetcher with the required dependencies.
     * @param client Reference to the singleton APIClient.
     * @param db     Reference to the singleton DatabaseClient.
     */
    PlayerFetcher(APIClient& client, DatabaseClient& db);

    /**
     * @brief Fetches players from the API and inserts new ones into MongoDB.
     *
     * Queries several top-flight competition endpoints, iterates over each
     * team's squad, and inserts any player not already in the database.
     * Stops once at least @p limit new players have been cached.
     *
     * @param limit Minimum number of new players to cache (default: 100).
     * @return Number of players newly inserted in this call.
     */
    int fetchAndCachePlayers(int limit = 100);
};