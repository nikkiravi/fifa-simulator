// src/packs/GoldPack.cpp
#include "packs/GoldPack.h"

std::vector<std::unique_ptr<Card>> GoldPack::open() {
    auto cards = getRandomCards(CardRarity::GOLD, 2);
    auto silver = getRandomCards(CardRarity::SILVER, 2);
    auto bronze = getRandomCards(CardRarity::BRONZE, 1);
    for (auto& c : silver) cards.push_back(std::move(c));
    for (auto& c : bronze) cards.push_back(std::move(c));
    return cards;
}