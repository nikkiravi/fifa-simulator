// src/packs/PremiumGoldPack.cpp
#include "packs/PremiumGoldPack.h"

std::vector<std::unique_ptr<Card>> PremiumGoldPack::open() {
    auto cards = getRandomCards(CardRarity::GOLD, 3);
    auto silver = getRandomCards(CardRarity::SILVER, 2);
    for (auto& c : silver) cards.push_back(std::move(c));
    return cards;
}