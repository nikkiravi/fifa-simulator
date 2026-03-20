// include/models/BronzeCard.h
#pragma once
#include "models/Card.h"

/**
 * @brief Bronze-tier player card (OVR 55–64).
 *
 * BronzeCard renders the card with bronze styling when displayCard()
 * is called.  It is created by BronzePack and stored in the user's Club.
 */
class BronzeCard : public Card {
public:
    /**
     * @brief Constructs a BronzeCard wrapping the given player.
     * @param player Shared pointer to the player this card represents.
     */
    explicit BronzeCard(std::shared_ptr<Player> player);

    /**
     * @brief Prints a bronze-styled card summary to stdout.
     */
    void displayCard() const override;

    /**
     * @return "Bronze"
     */
    std::string getRarityString() const override { return "Bronze"; }
};