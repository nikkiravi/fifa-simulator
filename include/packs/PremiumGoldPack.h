// include/packs/PremiumGoldPack.h
#pragma once
#include "packs/Pack.h"

/**
 * @brief Premium Gold pack containing 3 gold and 2 silver player cards (no bronze guaranteed).
 *
 * PremiumGoldPack is the highest-tier pack offered by the store.  It
 * guarantees 3 gold cards (OVR 75–99) and 2 silver cards (OVR 65–74).
 * Unlike GoldPack, no bronze card slot exists, making this pack the
 * best source of high-rated players.
 */
class PremiumGoldPack : public Pack {
public:
    /**
     * @brief Opens the pack and returns 5 cards (3 gold + 2 silver).
     * @return Vector of 5 unique_ptr<Card>.
     */
    std::vector<std::unique_ptr<Card>> open() override;

    /**
     * @return "Premium Gold Pack"
     */
    std::string getPackName() const override { return "Premium Gold Pack"; }

    /**
     * @return Description of the pack's contents.
     */
    std::string getPackDescription() const override {
        return "Contains 3 gold and 2 silver player cards.";
    }
};