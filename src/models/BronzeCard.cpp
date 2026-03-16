// src/models/BronzeCard.cpp
#include "models/BronzeCard.h"
#include <iostream>

BronzeCard::BronzeCard(std::shared_ptr<Player> player)
    : Card(std::move(player), CardRarity::BRONZE) {}

void BronzeCard::displayCard() const {
    std::cout << "[BRONZE] ";
    player->displayInfo();
}