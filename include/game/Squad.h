// include/game/Squad.h
#pragma once
#include <array>
#include <string>
#include <memory>
#include <vector>
#include "models/Card.h"
#include "game/Formation.h"
#include "database/Database.h"

// Squad manages 11 player cards arranged in a Formation.
//
// Ownership model:
//   - When building a squad from Club cards, slots hold non-owning raw pointers
//     (the Club's unique_ptrs remain the owners).
//   - When a squad is loaded from MongoDB, the Squad creates its own Card objects
//     stored in ownedCards; slots then point into that vector.
class Squad {
private:
    std::string name;
    Formation formation;
    std::array<Card*, 11> slots;               // non-owning (see ownership note above)
    std::vector<std::unique_ptr<Card>> ownedCards; // used only when loaded from DB

    // Reconstruct a Card from stored player data (mirrors Club::cardFromData)
    std::unique_ptr<Card> cardFromData(const UserCardData& data) const;

    // Build the BSON document used to persist this squad
    bsoncxx::v_noabi::document::value toDocument() const;

public:
    Squad(const std::string& name, FormationType formationType);

    // Add a card to a slot [0-10].
    // Validates that the card's position matches the formation slot requirement.
    // Returns true on success, false if the position doesn't match.
    bool addPlayer(int slotIndex, Card* card);

    // Clear a slot (does NOT delete the card).
    void removePlayer(int slotIndex);

    // Average overall rating of all filled slots (returns 0 if squad is empty).
    int calculateOverallRating() const;

    // Returns true only when all 11 slots are filled.
    bool isComplete() const;

    // Persist squad to the "squads" MongoDB collection.
    void saveToDatabase(DatabaseClient& db) const;

    // Load a previously saved squad by name.
    // The returned Squad owns its cards internally via ownedCards.
    static Squad loadFromDatabase(const std::string& squadName, DatabaseClient& db);

    void displaySquad() const;

    const std::string& getName() const { return name; }
    const Formation& getFormation() const { return formation; }
    Card* getSlot(int index) const { return slots[index]; }
};