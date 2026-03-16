// src/models/GoldCard.cpp
#include "models/GoldCard.h"
#include <iostream>

GoldCard::GoldCard(std::shared_ptr<Player> player)
    : Card(std::move(player), CardRarity::GOLD) {}

void GoldCard::displayCard() const {
    std::cout << "[GOLD]   ";
    player->displayInfo();
}