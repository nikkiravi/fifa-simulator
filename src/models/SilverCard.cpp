// src/models/SilverCard.cpp
#include "models/SilverCard.h"
#include <iostream>

SilverCard::SilverCard(std::shared_ptr<Player> player)
    : Card(std::move(player), CardRarity::SILVER) {}

void SilverCard::displayCard() const {
    std::cout << "[SILVER] ";
    player->displayInfo();
}