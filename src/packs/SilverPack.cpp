// src/packs/SilverPack.cpp
#include "packs/SilverPack.h"

std::vector<std::unique_ptr<Card>> SilverPack::open() {
    auto cards = getRandomCards(CardRarity::SILVER, 3);
    auto bronze = getRandomCards(CardRarity::BRONZE, 2);
    for (auto& c : bronze) {
        cards.push_back(std::move(c));
    }
    return cards;
}