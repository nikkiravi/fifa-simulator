// include/models/Player.h
#pragma once
#include <string>
#include <iostream>

/**
 * @brief Abstract base class representing a FIFA player card.
 *
 * Player stores the core attributes shared by every player type:
 * biographical data (name, nationality, club, position) and the six
 * standard FIFA statistics (pace, shooting, passing, dribbling,
 * defending, physicality).
 *
 * Concrete subclasses — GKPlayer, DefenderPlayer, MidfielderPlayer,
 * and AttackerPlayer — override displayInfo() to highlight the stats
 * most relevant to each position.
 *
 * @note This class is abstract; it cannot be instantiated directly.
 */
class Player {
protected:
    std::string name;         ///< Full player name.
    std::string nationality;  ///< Player's country / nationality.
    std::string club;         ///< Club the player belongs to.
    std::string position;     ///< "Goalkeeper", "Defence", "Midfield", or "Attack".

    int overall_rating; ///< Overall FIFA rating (0–99).
    int pace;           ///< Pace attribute (0–99).
    int shooting;       ///< Shooting attribute (0–99).
    int passing;        ///< Passing attribute (0–99).
    int dribbling;      ///< Dribbling attribute (0–99).
    int defending;      ///< Defending attribute (0–99).
    int physicality;    ///< Physicality attribute (0–99).

public:
    /**
     * @brief Constructs a Player with all required attributes.
     * @param name          Full player name.
     * @param nationality   Player's country / nationality.
     * @param club          Club the player belongs to.
     * @param position      Position string: "Goalkeeper", "Defence",
     *                      "Midfield", or "Attack".
     * @param overall_rating Overall FIFA rating (0–99).
     * @param pace           Pace attribute (0–99).
     * @param shooting       Shooting attribute (0–99).
     * @param passing        Passing attribute (0–99).
     * @param dribbling      Dribbling attribute (0–99).
     * @param defending      Defending attribute (0–99).
     * @param physicality    Physicality attribute (0–99).
     */
    Player(const std::string& name, const std::string& nationality,
           const std::string& club, const std::string& position,
           int overall_rating, int pace, int shooting, int passing,
           int dribbling, int defending, int physicality);

    virtual ~Player() = default;

    /**
     * @brief Displays position-specific player information to stdout.
     *
     * Pure virtual — each subclass prints the attributes most relevant
     * to its position (e.g., GKPlayer emphasises defending; AttackerPlayer
     * emphasises shooting and pace).
     */
    virtual void displayInfo() const = 0;

    // ── Getters ───────────────────────────────────────────────────────────────

    std::string getName()        const { return name; }           ///< @return Full player name.
    std::string getNationality() const { return nationality; }    ///< @return Player's nationality.
    std::string getClub()        const { return club; }           ///< @return Club name.
    std::string getPosition()    const { return position; }       ///< @return Position string.
    int getOverallRating()       const { return overall_rating; } ///< @return Overall rating (0–99).
    int getPace()                const { return pace; }           ///< @return Pace (0–99).
    int getShooting()            const { return shooting; }       ///< @return Shooting (0–99).
    int getPassing()             const { return passing; }        ///< @return Passing (0–99).
    int getDribbling()           const { return dribbling; }      ///< @return Dribbling (0–99).
    int getDefending()           const { return defending; }      ///< @return Defending (0–99).
    int getPhysicality()         const { return physicality; }    ///< @return Physicality (0–99).
};