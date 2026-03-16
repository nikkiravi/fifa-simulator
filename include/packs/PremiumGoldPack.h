// include/packs/PremiumGoldPack.h
#pragma once
#include "packs/Pack.h"

// Contains 3 gold + 2 silver cards (no bronze guaranteed)
class PremiumGoldPack : public Pack {
public:
    std::vector<std::unique_ptr<Card>> open() override;
    std::string getPackName() const override { return "Premium Gold Pack"; }
    std::string getPackDescription() const override { return "Contains 3 gold and 2 silver player cards."; }
};