// include/models/BronzeCard.h
#pragma once
#include "models/Card.h"

class BronzeCard : public Card {
public:
    BronzeCard(std::shared_ptr<Player> player);
    void displayCard() const override;
    std::string getRarityString() const override { return "Bronze"; }
};