// include/packs/Pack.h
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "models/Card.h"
#include "database/Database.h"

/**
 * @brief Abstract base class for all pack types (Template Method pattern).
 *
 * A Pack represents a FIFA Ultimate Team pack that the user can open.
 * Each concrete subclass defines how many cards of each rarity it
 * contains by implementing the open() method.
 *
 * The protected helper getRandomCards() queries MongoDB for players in
 * the appropriate rating band, shuffles them, and wraps the selected
 * players in the correct Card subclass.
 *
 * Concrete subclasses: BronzePack, SilverPack, GoldPack, PremiumGoldPack.
 *
 * @note Use PackFactory::createPack() rather than constructing packs directly.
 */
class Pack {
public:
    virtual ~Pack() = default;

    /**
     * @brief Opens the pack and returns the cards inside.
     *
     * Pure virtual — each subclass specifies its own card composition
     * (e.g., BronzePack returns 5 bronze cards; SilverPack returns a
     * mix of silver and bronze cards).
     *
     * @return Vector of unique_ptr<Card> transferred to the caller.
     */
    virtual std::vector<std::unique_ptr<Card>> open() = 0;

    /**
     * @brief Returns the display name of this pack type.
     * @return Human-readable name (e.g., "Bronze Pack", "Gold Pack").
     */
    virtual std::string getPackName() const = 0;

    /**
     * @brief Returns a short description of the pack's contents.
     * @return Description string shown in the UI (e.g., "Contains 5 bronze player cards.").
     */
    virtual std::string getPackDescription() const = 0;

protected:
    /**
     * @brief Queries MongoDB for random players in the rarity's rating range.
     *
     * Fetches all players from the database whose overall rating falls
     * within the rarity's band, shuffles the list, and wraps the first
     * @p count players in the appropriate Card subclass.
     *
     * @param rarity Determines the rating band (BRONZE=55–64, SILVER=65–74, GOLD=75–99).
     * @param count  Number of cards to return.
     * @return Vector of up to @p count unique_ptr<Card> objects.
     */
    static std::vector<std::unique_ptr<Card>> getRandomCards(CardRarity rarity, int count);

    /**
     * @brief Constructs the correct Player subclass from a PlayerData struct.
     *
     * Maps the position string in @p pd to the appropriate concrete
     * Player subclass (GKPlayer, DefenderPlayer, MidfielderPlayer,
     * or AttackerPlayer).
     *
     * @param pd Struct containing all player attributes fetched from MongoDB.
     * @return shared_ptr to the constructed Player.
     */
    static std::shared_ptr<Player> buildPlayer(const PlayerData& pd);
};