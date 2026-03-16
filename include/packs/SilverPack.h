// include/packs/SilverPack.h
#pragma once
#include "packs/Pack.h"

// Contains 3 silver + 2 bronze cards
class SilverPack : public Pack {
public:
    std::vector<std::unique_ptr<Card>> open() override;
    std::string getPackName() const override { return "Silver Pack"; }
    std::string getPackDescription() const override { return "Contains 3 silver and 2 bronze player cards."; }
};