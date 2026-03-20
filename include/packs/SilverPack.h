// include/packs/SilverPack.h
#pragma once
#include "packs/Pack.h"

/**
 * @brief Silver pack containing 3 silver and 2 bronze player cards.
 *
 * SilverPack is the mid-tier pack.  It guarantees at least 3 silver
 * cards (OVR 65–74) and fills the remaining 2 slots with bronze cards
 * (OVR 55–64).
 */
class SilverPack : public Pack {
public:
    /**
     * @brief Opens the pack and returns 5 cards (3 silver + 2 bronze).
     * @return Vector of 5 unique_ptr<Card>.
     */
    std::vector<std::unique_ptr<Card>> open() override;

    /**
     * @return "Silver Pack"
     */
    std::string getPackName() const override { return "Silver Pack"; }

    /**
     * @return Description of the pack's contents.
     */
    std::string getPackDescription() const override {
        return "Contains 3 silver and 2 bronze player cards.";
    }
};