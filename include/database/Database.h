// include/database/Database.h
#pragma once
#include <string>
#include <vector>
#include <mongocxx/v_noabi/mongocxx/client.hpp>
#include <mongocxx/v_noabi/mongocxx/instance.hpp>
#include <mongocxx/v_noabi/mongocxx/database.hpp>
#include <bsoncxx/v_noabi/bsoncxx/document/value.hpp>

/**
 * @brief Plain data transfer struct for a player record fetched from MongoDB.
 *
 * PlayerData is used internally by Pack classes (to build Card objects) and
 * by PlayerFetcher (to insert new records).  All fields correspond directly
 * to fields in the "players" MongoDB collection.
 */
struct PlayerData {
    std::string name;        ///< Full player name.
    std::string nationality; ///< Player's country / nationality.
    std::string club;        ///< Club name.
    std::string position;    ///< "Goalkeeper", "Defence", "Midfield", or "Attack".
    int rating;              ///< Overall rating (0–99).
    int pace;                ///< Pace attribute (0–99).
    int shooting;            ///< Shooting attribute (0–99).
    int passing;             ///< Passing attribute (0–99).
    int dribbling;           ///< Dribbling attribute (0–99).
    int defending;           ///< Defending attribute (0–99).
    int physical;            ///< Physicality attribute (0–99).
};

/**
 * @brief Extends PlayerData with a rarity label for user-owned cards.
 *
 * UserCardData is used by Club and Squad when reading from or writing to
 * the "user_cards" MongoDB collection.
 */
struct UserCardData : PlayerData {
    std::string rarity; ///< Card rarity string: "Bronze", "Silver", or "Gold".
};

/**
 * @brief Aggregates all data for one saved squad document.
 *
 * SquadData is populated by DatabaseClient::getSquadByName() and consumed
 * by Squad::loadFromDatabase() to reconstruct a Squad object.
 */
struct SquadData {
    std::string squadName;             ///< Name of the squad.
    std::string formation;             ///< Formation string (e.g., "4-3-3").
    std::vector<UserCardData> players; ///< 11-element array, indexed by slot; empty name = empty slot.
};

/**
 * @brief MongoDB database client (Singleton pattern).
 *
 * DatabaseClient provides a single, shared connection to the fifa_simulator
 * MongoDB database.  It exposes typed CRUD methods for each of the three
 * collections the application uses:
 *
 *  - **players**    — master player roster fetched from the football-data.org API.
 *  - **user_cards** — cards the user has collected by opening packs.
 *  - **squads**     — saved squad configurations.
 *
 * Obtain the instance with DatabaseClient::getInstance().  Copying is disabled.
 *
 * @note The MongoDB instance must be running on the URI provided to getInstance()
 *       (default: mongodb://localhost:27017) before any method is called.
 */
class DatabaseClient {
private:
    static DatabaseClient* instance;           ///< The singleton instance.
    mongocxx::v_noabi::instance mongoInstance; ///< mongocxx driver initialisation guard.
    mongocxx::v_noabi::client client;          ///< Active MongoDB client connection.
    mongocxx::v_noabi::database db;            ///< Handle to the fifa_simulator database.

    DatabaseClient(const std::string& uri, const std::string& dbName);

public:
    /**
     * @brief Returns the singleton DatabaseClient, creating it on first call.
     * @param uri    MongoDB connection URI (default: "mongodb://localhost:27017").
     * @param dbName Database name to use (default: "fifa_simulator").
     * @return Reference to the singleton DatabaseClient.
     */
    static DatabaseClient& getInstance(
        const std::string& uri    = "mongodb://localhost:27017",
        const std::string& dbName = "fifa_simulator");

    // ── players collection (Milestone 3) ─────────────────────────────────────

    /**
     * @brief Inserts a player document into the "players" collection.
     * @param doc BSON document to insert.
     */
    void insertPlayer(const bsoncxx::v_noabi::document::value& doc);

    /**
     * @brief Checks whether a player with the given name already exists.
     * @param playerName Player name to look up.
     * @return true if at least one matching document exists.
     */
    bool playerExists(std::string& playerName);

    /**
     * @return Total number of documents in the "players" collection.
     */
    long getPlayerCount();

    // ── players collection — pack queries (Milestone 4) ───────────────────────

    /**
     * @brief Returns all players whose overall rating is in [minRating, maxRating].
     * @param minRating Lower rating bound (inclusive).
     * @param maxRating Upper rating bound (inclusive).
     * @return Vector of PlayerData structs.
     */
    std::vector<PlayerData> getPlayerDataByRatingRange(int minRating, int maxRating);

    // ── user_cards collection (Milestone 4) ───────────────────────────────────

    /**
     * @brief Inserts a user card document into the "user_cards" collection.
     * @param doc BSON document to insert.
     */
    void insertUserCard(const bsoncxx::v_noabi::document::value& doc);

    /**
     * @return All documents in the "user_cards" collection as UserCardData structs.
     */
    std::vector<UserCardData> getUserCardData();

    /**
     * @return Total number of documents in the "user_cards" collection.
     */
    long getUserCardCount();

    // ── squads collection (Milestone 5) ──────────────────────────────────────

    /**
     * @brief Inserts (or replaces) a squad document in the "squads" collection.
     * @param doc BSON document representing the squad.
     */
    void insertSquad(const bsoncxx::v_noabi::document::value& doc);

    /**
     * @brief Retrieves a saved squad by name.
     * @param squadName Name field to match.
     * @return SquadData struct populated from the matching document.
     * @throws std::runtime_error if no squad with @p squadName is found.
     */
    SquadData getSquadByName(const std::string& squadName);

    /**
     * @return Names of all squads currently stored in the "squads" collection.
     */
    std::vector<std::string> getAllSquadNames();

    // Copying is disabled — only one instance should ever exist.
    DatabaseClient(const DatabaseClient&)            = delete;
    DatabaseClient& operator=(const DatabaseClient&) = delete;
};