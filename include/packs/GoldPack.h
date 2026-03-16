// include/packs/GoldPack.h
#pragma once
#include "packs/Pack.h"

// Contains 2 gold + 2 silver + 1 bronze card
class GoldPack : public Pack {
public:
    std::vector<std::unique_ptr<Card>> open() override;
    std::string getPackName() const override { return "Gold Pack"; }
    std::string getPackDescription() const override { return "Contains 2 gold, 2 silver, and 1 bronze player card."; }
};