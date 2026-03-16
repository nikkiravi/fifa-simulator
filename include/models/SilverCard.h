// include/models/SilverCard.h
#pragma once
#include "models/Card.h"

class SilverCard : public Card {
public:
    SilverCard(std::shared_ptr<Player> player);
    void displayCard() const override;
    std::string getRarityString() const override { return "Silver"; }
};