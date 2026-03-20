// include/models/DefenderPlayer.h
#pragma once
#include "Player.h"
#include <string>
#include <iostream>

/**
 * @brief Concrete Player subclass for defenders.
 *
 * DefenderPlayer overrides displayInfo() to highlight the attributes
 * most relevant to the Defence position: defending and physicality.
 * All other player data is managed by the Player base class.
 */
class DefenderPlayer : public Player {
public:
    /**
     * @brief Constructs a DefenderPlayer with full player attributes.
     * @param name          Full player name.
     * @param nationality   Player's country / nationality.
     * @param club          Club the player belongs to.
     * @param position      Expected to be "Defence".
     * @param overall_rating Overall FIFA rating (0–99).
     * @param pace           Pace attribute (0–99).
     * @param shooting       Shooting attribute (0–99).
     * @param passing        Passing attribute (0–99).
     * @param dribbling      Dribbling attribute (0–99).
     * @param defending      Defending attribute (0–99) — primary defender stat.
     * @param physicality    Physicality attribute (0–99).
     */
    DefenderPlayer(const std::string& name, const std::string& nationality,
                   const std::string& club, const std::string& position,
                   int overall_rating, int pace, int shooting, int passing,
                   int dribbling, int defending, int physicality);

    /**
     * @brief Displays defender-specific info (defending and physicality).
     */
    void displayInfo() const override;
};