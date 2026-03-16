// src/packs/PackFactory.cpp
#include "packs/PackFactory.h"
#include "packs/BronzePack.h"
#include "packs/SilverPack.h"
#include "packs/GoldPack.h"
#include "packs/PremiumGoldPack.h"

std::unique_ptr<Pack> PackFactory::createPack(PackType type) {
    switch (type) {
        case PackType::BRONZE:       return std::make_unique<BronzePack>();
        case PackType::SILVER:       return std::make_unique<SilverPack>();
        case PackType::GOLD:         return std::make_unique<GoldPack>();
        case PackType::PREMIUM_GOLD: return std::make_unique<PremiumGoldPack>();
        default:                     return std::make_unique<GoldPack>();
    }
}

std::unique_ptr<Pack> PackFactory::createPack(const std::string& typeName) {
    if (typeName == "Bronze")      return std::make_unique<BronzePack>();
    if (typeName == "Silver")      return std::make_unique<SilverPack>();
    if (typeName == "Gold")        return std::make_unique<GoldPack>();
    if (typeName == "PremiumGold") return std::make_unique<PremiumGoldPack>();
    return std::make_unique<GoldPack>(); // default fallback
}