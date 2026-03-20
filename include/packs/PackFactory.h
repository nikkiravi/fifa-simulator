// include/packs/PackFactory.h
#pragma once
#include <memory>
#include <string>
#include "packs/Pack.h"

/**
 * @brief Identifies the type of pack to create.
 *
 * Passed to PackFactory::createPack() to select the concrete Pack subclass
 * without exposing the subclass names to the caller.
 */
enum class PackType {
    BRONZE,       ///< BronzePack  — 5 bronze cards.
    SILVER,       ///< SilverPack  — 3 silver + 2 bronze cards.
    GOLD,         ///< GoldPack    — 2 gold + 2 silver + 1 bronze card.
    PREMIUM_GOLD  ///< PremiumGoldPack — 3 gold + 2 silver cards.
};

/**
 * @brief Factory for creating Pack objects (Factory pattern).
 *
 * PackFactory centralises Pack instantiation so callers never need to
 * include or name concrete pack headers.  Two overloads are provided:
 * one taking a PackType enum and one taking a string name.
 *
 * Example usage:
 * @code
 *   auto pack = PackFactory::createPack(PackType::GOLD);
 *   auto cards = pack->open();
 * @endcode
 */
class PackFactory {
public:
    /**
     * @brief Creates a Pack of the specified type.
     * @param type Enum value identifying the desired pack.
     * @return unique_ptr to the constructed Pack subclass.
     * @throws std::invalid_argument if @p type is unrecognised.
     */
    static std::unique_ptr<Pack> createPack(PackType type);

    /**
     * @brief Creates a Pack matching the given type name string.
     *
     * Accepts "Bronze", "Silver", "Gold", and "PremiumGold" (case-sensitive).
     *
     * @param typeName Human-readable pack type name.
     * @return unique_ptr to the constructed Pack subclass.
     * @throws std::invalid_argument if @p typeName is unrecognised.
     */
    static std::unique_ptr<Pack> createPack(const std::string& typeName);
};