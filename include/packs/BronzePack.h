// include/packs/BronzePack.h
#pragma once
#include "packs/Pack.h"

// Contains 5 bronze cards
class BronzePack : public Pack {
public:
    std::vector<std::unique_ptr<Card>> open() override;
    std::string getPackName() const override { return "Bronze Pack"; }
    std::string getPackDescription() const override { return "Contains 5 bronze player cards."; }
};