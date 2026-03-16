// include/models/Card.h
#pragma once
#include <string>
#include <memory>
#include "models/Player.h"

enum class CardRarity {
    BRONZE,
    SILVER,
    GOLD
};

// Card HAS-A Player (composition, not inheritance)
class Card {
protected:
    std::shared_ptr<Player> player;
    CardRarity rarity;

public:
    Card(std::shared_ptr<Player> player, CardRarity rarity);
    virtual ~Card() = default;

    virtual void displayCard() const = 0;
    virtual std::string getRarityString() const = 0;

    std::shared_ptr<Player> getPlayer() const { return player; }
    CardRarity getRarity() const { return rarity; }
};