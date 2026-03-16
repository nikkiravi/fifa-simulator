// include/packs/Pack.h
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "models/Card.h"
#include "database/Database.h"

// Abstract base class for all pack types
class Pack {
public:
    virtual ~Pack() = default;

    // Opens the pack and returns the cards inside (pure virtual)
    virtual std::vector<std::unique_ptr<Card>> open() = 0;

    virtual std::string getPackName() const = 0;
    virtual std::string getPackDescription() const = 0;

protected:
    // Helper: queries DB for players in the given rarity's rating range,
    // shuffles the results, and wraps the first 'count' players in Card objects.
    static std::vector<std::unique_ptr<Card>> getRandomCards(CardRarity rarity, int count);

    // Helper: constructs the correct Player subclass from a PlayerData struct
    static std::shared_ptr<Player> buildPlayer(const PlayerData& pd);
};