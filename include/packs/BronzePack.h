// include/packs/BronzePack.h
#pragma once
#include "packs/Pack.h"

/**
 * @brief Bronze pack containing 5 bronze-tier player cards (OVR 55–64).
 *
 * BronzePack is the entry-level pack type.  All five cards are drawn
 * from the bronze rating band (55–64 OVR).
 */
class BronzePack : public Pack {
public:
    /**
     * @brief Opens the pack and returns 5 BronzeCard objects.
     * @return Vector of 5 unique_ptr<Card> (all bronze rarity).
     */
    std::vector<std::unique_ptr<Card>> open() override;

    /**
     * @return "Bronze Pack"
     */
    std::string getPackName() const override { return "Bronze Pack"; }

    /**
     * @return Description of the pack's contents.
     */
    std::string getPackDescription() const override {
        return "Contains 5 bronze player cards.";
    }
};