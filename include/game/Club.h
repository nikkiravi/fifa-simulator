// include/game/Club.h
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "models/Card.h"
#include "database/Database.h"

/**
 * @brief Manages the user's card collection (in-memory + MongoDB persistence).
 *
 * Club is the central repository for all cards the user has obtained by
 * opening packs.  Cards are stored as unique_ptr<Card> in an in-memory
 * vector and simultaneously persisted to the "user_cards" MongoDB collection.
 *
 * Key responsibilities:
 *  - **Adding cards**: addCards() persists each card to MongoDB and appends it
 *    to the in-memory collection.
 *  - **Loading**: loadFromDatabase() rebuilds the in-memory collection from
 *    MongoDB on startup (or after any external modification).
 *  - **Search / Filter**: provides eight query methods that return raw (non-owning)
 *    pointers into the collection for read-only use.
 *  - **Sort**: sortByRating() and sortByPosition() reorder the in-memory vector.
 *  - **Display**: displayCollection() and displayStats() print formatted output.
 */
class Club {
private:
    std::vector<std::unique_ptr<Card>> collection; ///< In-memory card collection.
    DatabaseClient& db;                            ///< Non-owning reference to the DB singleton.

    // Internal helpers for DB <-> Card round-tripping
    std::unique_ptr<Card> cardFromData(const UserCardData& data) const;
    bsoncxx::v_noabi::document::value cardToDocument(const Card& card) const;

public:
    /**
     * @brief Constructs a Club backed by the given database connection.
     * @param db Reference to the active DatabaseClient singleton.
     */
    explicit Club(DatabaseClient& db);

    /**
     * @brief Persists a batch of opened cards and adds them to the collection.
     *
     * Each card is written to the "user_cards" MongoDB collection and then
     * appended to the in-memory collection.  Ownership of the unique_ptrs
     * is transferred from the caller to the Club.
     *
     * @param cards Cards received from Pack::open().
     */
    void addCards(std::vector<std::unique_ptr<Card>> cards);

    /**
     * @brief Reloads the entire collection from MongoDB.
     *
     * Clears the in-memory collection and reconstructs every card from
     * the "user_cards" collection.  Call this after any external writes
     * (e.g., on application startup).
     */
    void loadFromDatabase();

    // ── Search (non-owning raw pointers; ownership stays in collection) ────────

    /**
     * @brief Finds cards whose player name contains @p name (case-insensitive).
     * @param name Substring to search for.
     * @return Raw pointers to matching cards.
     */
    std::vector<Card*> searchByName(const std::string& name) const;

    /**
     * @brief Finds cards whose player position exactly matches @p position.
     * @param position "Goalkeeper", "Defence", "Midfield", or "Attack".
     * @return Raw pointers to matching cards.
     */
    std::vector<Card*> searchByPosition(const std::string& position) const;

    /**
     * @brief Finds cards whose player club exactly matches @p clubName.
     * @param clubName Exact club name string.
     * @return Raw pointers to matching cards.
     */
    std::vector<Card*> searchByClub(const std::string& clubName) const;

    /**
     * @brief Finds cards whose player nationality exactly matches @p nationality.
     * @param nationality Exact nationality string.
     * @return Raw pointers to matching cards.
     */
    std::vector<Card*> searchByNationality(const std::string& nationality) const;

    // ── Filter ────────────────────────────────────────────────────────────────

    /**
     * @brief Returns cards with an overall rating in [minRating, maxRating].
     * @param minRating Lower bound (inclusive).
     * @param maxRating Upper bound (inclusive).
     * @return Raw pointers to matching cards.
     */
    std::vector<Card*> filterByRating(int minRating, int maxRating) const;

    /**
     * @brief Returns cards of the specified rarity tier.
     * @param rarity CardRarity::BRONZE, SILVER, or GOLD.
     * @return Raw pointers to matching cards.
     */
    std::vector<Card*> filterByRarity(CardRarity rarity) const;

    // ── Sort (modifies in-memory order only) ──────────────────────────────────

    /**
     * @brief Sorts the in-memory collection by overall rating (descending).
     */
    void sortByRating();

    /**
     * @brief Sorts the in-memory collection by position string (ascending).
     */
    void sortByPosition();

    // ── Display ───────────────────────────────────────────────────────────────

    /**
     * @brief Prints every card in the current in-memory order to stdout.
     */
    void displayCollection() const;

    /**
     * @brief Prints aggregate statistics (counts by rarity, avg/best rating).
     */
    void displayStats() const;

    /**
     * @return Number of cards currently in the in-memory collection.
     */
    int getCardCount() const { return static_cast<int>(collection.size()); }
};