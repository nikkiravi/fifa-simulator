// include/models/GoldCard.h
#pragma once
#include "models/Card.h"

/**
 * @brief Gold-tier player card (OVR 75–99).
 *
 * GoldCard renders the card with gold styling when displayCard()
 * is called.  It is created by GoldPack and PremiumGoldPack and
 * stored in the user's Club.
 */
class GoldCard : public Card {
public:
    /**
     * @brief Constructs a GoldCard wrapping the given player.
     * @param player Shared pointer to the player this card represents.
     */
    explicit GoldCard(std::shared_ptr<Player> player);

    /**
     * @brief Prints a gold-styled card summary to stdout.
     */
    void displayCard() const override;

    /**
     * @return "Gold"
     */
    std::string getRarityString() const override { return "Gold"; }
};