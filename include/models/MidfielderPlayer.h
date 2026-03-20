// include/models/MidfielderPlayer.h
#pragma once
#include "Player.h"
#include <string>
#include <iostream>

/**
 * @brief Concrete Player subclass for midfielders.
 *
 * MidfielderPlayer overrides displayInfo() to highlight the attributes
 * most relevant to the Midfield position: passing and dribbling.
 * All other player data is managed by the Player base class.
 */
class MidfielderPlayer : public Player {
public:
    /**
     * @brief Constructs a MidfielderPlayer with full player attributes.
     * @param name          Full player name.
     * @param nationality   Player's country / nationality.
     * @param club          Club the player belongs to.
     * @param position      Expected to be "Midfield".
     * @param overall_rating Overall FIFA rating (0–99).
     * @param pace           Pace attribute (0–99).
     * @param shooting       Shooting attribute (0–99).
     * @param passing        Passing attribute (0–99) — key midfielder stat.
     * @param dribbling      Dribbling attribute (0–99) — key midfielder stat.
     * @param defending      Defending attribute (0–99).
     * @param physicality    Physicality attribute (0–99).
     */
    MidfielderPlayer(const std::string& name, const std::string& nationality,
                     const std::string& club, const std::string& position,
                     int overall_rating, int pace, int shooting, int passing,
                     int dribbling, int defending, int physicality);

    /**
     * @brief Displays midfielder-specific info (passing and dribbling).
     */
    void displayInfo() const override;
};