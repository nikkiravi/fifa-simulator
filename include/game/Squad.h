// include/game/Squad.h
#pragma once
#include <array>
#include <string>
#include <memory>
#include <vector>
#include "models/Card.h"
#include "game/Formation.h"
#include "database/Database.h"

/**
 * @brief Manages an 11-player squad arranged according to a Formation.
 *
 * Squad holds 11 card slots (indexed 0–10) whose required positions are
 * dictated by its Formation.  addPlayer() enforces positional constraints:
 * a card can only be placed in a slot whose required position matches the
 * card's player position.
 *
 * **Ownership model:**
 *  - When a squad is built from Club cards (e.g., in the Menu's squad
 *    builder), slots hold *non-owning* raw pointers — the Club's
 *    unique_ptrs remain the owners.
 *  - When a squad is loaded from MongoDB via loadFromDatabase(), the Squad
 *    reconstructs Card objects and stores them in `ownedCards`; slots then
 *    point into that vector.
 *
 * Squads can be persisted to and retrieved from the "squads" MongoDB
 * collection via saveToDatabase() and loadFromDatabase().
 */
class Squad {
private:
    std::string name;                               ///< User-defined squad name.
    Formation formation;                            ///< Tactical formation for this squad.
    std::array<Card*, 11> slots;                   ///< Non-owning slot pointers (see ownership note).
    std::vector<std::unique_ptr<Card>> ownedCards; ///< Owned cards when loaded from DB.

    // Internal helpers
    std::unique_ptr<Card> cardFromData(const UserCardData& data) const;
    bsoncxx::v_noabi::document::value toDocument() const;

public:
    /**
     * @brief Constructs an empty Squad with the given name and formation.
     * @param name          User-defined squad name.
     * @param formationType The tactical formation to use.
     */
    Squad(const std::string& name, FormationType formationType);

    /**
     * @brief Places a card into the specified slot after validating its position.
     *
     * The card's player position must match the position required by the
     * Formation for that slot index.
     *
     * @param slotIndex Index of the slot to fill (0–10).
     * @param card      Non-owning pointer to the card to place.
     * @return true if the card was placed successfully; false on position mismatch
     *         or invalid slot index.
     */
    bool addPlayer(int slotIndex, Card* card);

    /**
     * @brief Clears the specified slot without deleting the card.
     * @param slotIndex Index of the slot to clear (0–10).
     */
    void removePlayer(int slotIndex);

    /**
     * @brief Computes the average overall rating of all filled slots.
     * @return Integer average rating, or 0 if the squad is completely empty.
     */
    int calculateOverallRating() const;

    /**
     * @brief Checks whether all 11 slots are filled.
     * @return true if every slot contains a card.
     */
    bool isComplete() const;

    /**
     * @brief Persists the squad to the "squads" MongoDB collection.
     *
     * If a squad with the same name already exists it will be overwritten
     * (upsert behaviour depends on DatabaseClient::insertSquad implementation).
     *
     * @param db Reference to the active DatabaseClient.
     */
    void saveToDatabase(DatabaseClient& db) const;

    /**
     * @brief Loads a previously saved squad from MongoDB by name.
     *
     * The returned Squad owns its cards internally via ownedCards, so
     * it is safe to use after the originating Club goes out of scope.
     *
     * @param squadName Name of the squad document to load.
     * @param db        Reference to the active DatabaseClient.
     * @return Fully initialised Squad object.
     * @throws std::runtime_error if no squad with @p squadName exists.
     */
    static Squad loadFromDatabase(const std::string& squadName, DatabaseClient& db);

    /**
     * @brief Prints a formatted squad roster to stdout.
     */
    void displaySquad() const;

    /**
     * @return The squad's name.
     */
    const std::string& getName() const { return name; }

    /**
     * @return Read-only reference to the squad's Formation.
     */
    const Formation& getFormation() const { return formation; }

    /**
     * @brief Returns the card in the specified slot (may be nullptr).
     * @param index Slot index (0–10).
     * @return Non-owning pointer to the card, or nullptr if the slot is empty.
     */
    Card* getSlot(int index) const { return slots[index]; }
};