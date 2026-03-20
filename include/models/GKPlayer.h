// include/models/GKPlayer.h
#pragma once
#include "models/Player.h"
#include <string>
#include <iostream>

/**
 * @brief Concrete Player subclass for goalkeepers.
 *
 * GKPlayer overrides displayInfo() to highlight the attributes most
 * relevant to the Goalkeeper position: pace and defending.
 * All other player data is managed by the Player base class.
 */
class GKPlayer : public Player {
public:
    /**
     * @brief Constructs a GKPlayer with full player attributes.
     * @param name          Full player name.
     * @param nationality   Player's country / nationality.
     * @param club          Club the player belongs to.
     * @param position      Expected to be "Goalkeeper".
     * @param overall_rating Overall FIFA rating (0–99).
     * @param pacing         Pace attribute (0–99).
     * @param shooting       Shooting attribute (0–99).
     * @param passing        Passing attribute (0–99).
     * @param dribbling      Dribbling attribute (0–99).
     * @param defending      Defending attribute (0–99) — primary GK stat.
     * @param physicality    Physicality attribute (0–99).
     */
    GKPlayer(const std::string& name, const std::string& nationality,
             const std::string& club, const std::string& position,
             int overall_rating, int pacing, int shooting, int passing,
             int dribbling, int defending, int physicality);

    /**
     * @brief Displays goalkeeper-specific info (pace and defending).
     */
    void displayInfo() const override;
};