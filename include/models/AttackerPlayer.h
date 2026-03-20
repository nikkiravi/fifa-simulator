// include/models/AttackerPlayer.h
#pragma once
#include "models/Player.h"
#include <string>
#include <iostream>

/**
 * @brief Concrete Player subclass for attackers.
 *
 * AttackerPlayer overrides displayInfo() to highlight the attributes
 * most relevant to the Attack position: pace and shooting.
 * All other player data is managed by the Player base class.
 */
class AttackerPlayer : public Player {
public:
    /**
     * @brief Constructs an AttackerPlayer with full player attributes.
     * @param name          Full player name.
     * @param nationality   Player's country / nationality.
     * @param club          Club the player belongs to.
     * @param position      Expected to be "Attack".
     * @param overall_rating Overall FIFA rating (0–99).
     * @param pace           Pace attribute (0–99) — key attacker stat.
     * @param shooting       Shooting attribute (0–99) — key attacker stat.
     * @param passing        Passing attribute (0–99).
     * @param dribbling      Dribbling attribute (0–99).
     * @param defending      Defending attribute (0–99).
     * @param physicality    Physicality attribute (0–99).
     */
    AttackerPlayer(const std::string& name, const std::string& nationality,
                   const std::string& club, const std::string& position,
                   int overall_rating, int pace, int shooting, int passing,
                   int dribbling, int defending, int physicality);

    /**
     * @brief Displays attacker-specific info (pace and shooting).
     */
    void displayInfo() const override;
};