// src/packs/BronzePack.cpp
#include "packs/BronzePack.h"

std::vector<std::unique_ptr<Card>> BronzePack::open() {
    return getRandomCards(CardRarity::BRONZE, 5);
}