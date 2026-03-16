// include/game/Club.h
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "models/Card.h"
#include "database/Database.h"

// Club manages the user's card collection.
// Cards are stored both in memory (for fast access) and in MongoDB (for persistence).
class Club {
private:
    std::vector<std::unique_ptr<Card>> collection;
    DatabaseClient& db;

    // Internal helpers for DB <-> Card conversion
    std::unique_ptr<Card> cardFromData(const UserCardData& data) const;
    bsoncxx::v_noabi::document::value cardToDocument(const Card& card) const;

public:
    explicit Club(DatabaseClient& db);

    // Persist and store a batch of opened cards
    void addCards(std::vector<std::unique_ptr<Card>> cards);

    // Reload the entire collection from MongoDB
    void loadFromDatabase();

    // --- Search (returns raw pointers; ownership stays in collection) ---
    std::vector<Card*> searchByName(const std::string& name) const;
    std::vector<Card*> searchByPosition(const std::string& position) const;
    std::vector<Card*> searchByClub(const std::string& clubName) const;
    std::vector<Card*> searchByNationality(const std::string& nationality) const;

    // --- Filter ---
    std::vector<Card*> filterByRating(int minRating, int maxRating) const;
    std::vector<Card*> filterByRarity(CardRarity rarity) const;

    // --- Sort (modifies in-memory order only) ---
    void sortByRating();
    void sortByPosition();

    // --- Display ---
    void displayCollection() const;
    void displayStats() const;

    int getCardCount() const { return static_cast<int>(collection.size()); }
};