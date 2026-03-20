// include/models/Card.h
#pragma once
#include <string>
#include <memory>
#include "models/Player.h"

/**
 * @brief Rarity tiers available for player cards.
 *
 * A card's rarity controls which rating band its player is drawn from:
 *  - BRONZE: OVR 55–64
 *  - SILVER: OVR 65–74
 *  - GOLD:   OVR 75–99
 */
enum class CardRarity {
    BRONZE, ///< Bronze tier — lower-rated players (OVR 55–64).
    SILVER, ///< Silver tier — mid-rated players   (OVR 65–74).
    GOLD    ///< Gold tier   — high-rated players  (OVR 75–99).
};

/**
 * @brief Abstract base class for all player cards (Composition pattern).
 *
 * Card wraps a Player via composition (HAS-A relationship) and adds a
 * CardRarity label. Concrete subclasses (BronzeCard, SilverCard, GoldCard)
 * implement displayCard() to render the card in the style appropriate to
 * its tier.
 *
 * Ownership: Card holds a shared_ptr<Player> so the same Player object
 * can safely be referenced from multiple places (e.g., a Squad slot and
 * a Club collection) without duplication.
 *
 * @note This class is abstract; use BronzeCard, SilverCard, or GoldCard.
 */
class Card {
protected:
    std::shared_ptr<Player> player; ///< The player data associated with this card.
    CardRarity rarity;              ///< The rarity tier of this card.

public:
    /**
     * @brief Constructs a Card wrapping the given player at the given rarity.
     * @param player Shared pointer to the Player this card represents.
     * @param rarity Rarity tier (BRONZE, SILVER, or GOLD).
     */
    Card(std::shared_ptr<Player> player, CardRarity rarity);
    virtual ~Card() = default;

    /**
     * @brief Prints a formatted card display to stdout.
     *
     * Pure virtual — each concrete subclass renders the card with styling
     * appropriate to its rarity tier.
     */
    virtual void displayCard() const = 0;

    /**
     * @brief Returns the rarity label as a human-readable string.
     * @return "Bronze", "Silver", or "Gold".
     */
    virtual std::string getRarityString() const = 0;

    /**
     * @brief Returns a shared pointer to the wrapped Player.
     * @return Shared pointer to the Player object.
     */
    std::shared_ptr<Player> getPlayer() const { return player; }

    /**
     * @brief Returns the card's rarity enum value.
     * @return CardRarity enum value for this card.
     */
    CardRarity getRarity() const { return rarity; }
};