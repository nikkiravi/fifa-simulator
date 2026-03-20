// include/packs/GoldPack.h
#pragma once
#include "packs/Pack.h"

/**
 * @brief Gold pack containing 2 gold, 2 silver, and 1 bronze player card.
 *
 * GoldPack is the standard high-tier pack.  It guarantees 2 gold cards
 * (OVR 75–99), 2 silver cards (OVR 65–74), and 1 bronze card (OVR 55–64).
 */
class GoldPack : public Pack {
public:
    /**
     * @brief Opens the pack and returns 5 cards (2 gold + 2 silver + 1 bronze).
     * @return Vector of 5 unique_ptr<Card>.
     */
    std::vector<std::unique_ptr<Card>> open() override;

    /**
     * @return "Gold Pack"
     */
    std::string getPackName() const override { return "Gold Pack"; }

    /**
     * @return Description of the pack's contents.
     */
    std::string getPackDescription() const override {
        return "Contains 2 gold, 2 silver, and 1 bronze player card.";
    }
};