// include/models/SilverCard.h
#pragma once
#include "models/Card.h"

/**
 * @brief Silver-tier player card (OVR 65–74).
 *
 * SilverCard renders the card with silver styling when displayCard()
 * is called.  It is created by SilverPack and stored in the user's Club.
 */
class SilverCard : public Card {
public:
    /**
     * @brief Constructs a SilverCard wrapping the given player.
     * @param player Shared pointer to the player this card represents.
     */
    explicit SilverCard(std::shared_ptr<Player> player);

    /**
     * @brief Prints a silver-styled card summary to stdout.
     */
    void displayCard() const override;

    /**
     * @return "Silver"
     */
    std::string getRarityString() const override { return "Silver"; }
};