// include/packs/PackFactory.h
#pragma once
#include <memory>
#include <string>
#include "packs/Pack.h"

enum class PackType {
    BRONZE,
    SILVER,
    GOLD,
    PREMIUM_GOLD
};

// Factory pattern: centralises Pack creation so callers don't need to
// know which concrete class to instantiate.
class PackFactory {
public:
    static std::unique_ptr<Pack> createPack(PackType type);
    static std::unique_ptr<Pack> createPack(const std::string& typeName);
};