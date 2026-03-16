// include/models/GoldCard.h
#pragma once
#include "models/Card.h"

class GoldCard : public Card {
public:
    GoldCard(std::shared_ptr<Player> player);
    void displayCard() const override;
    std::string getRarityString() const override { return "Gold"; }
};