// src/models/Card.cpp
#include "models/Card.h"

Card::Card(std::shared_ptr<Player> player, CardRarity rarity)
    : player(std::move(player)), rarity(rarity) {}