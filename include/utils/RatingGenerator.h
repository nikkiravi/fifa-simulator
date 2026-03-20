// include/utils/RatingGenerator.h
#pragma once
#include <string>
#include <nlohmann/json.hpp>

/**
 * @brief Generates realistic FIFA-style ratings and stats for players.
 *
 * The football-data.org API does not supply player ratings, so RatingGenerator
 * synthesises plausible overall ratings and per-attribute stats based on a
 * player's position.  Stat distributions are tuned so each position's primary
 * attributes skew higher than secondary ones, matching typical FIFA card values.
 *
 * All methods are static — no instance is needed.
 *
 * Position-to-stat mapping (primary attributes):
 *  - Goalkeeper  → pace, defending
 *  - Defence     → defending, physicality
 *  - Midfield    → passing, dribbling
 *  - Attack      → pace, shooting
 */
class RatingGenerator {
public:
    /**
     * @brief Generates an overall rating appropriate for the given position.
     *
     * Ratings are drawn from a position-specific range so that the overall
     * population of generated players covers the full 55–99 band seen in FIFA.
     *
     * @param position Position string: "Goalkeeper", "Defence", "Midfield", or "Attack".
     * @return Integer overall rating in [55, 99].
     */
    static int generateRating(const std::string& position);

    /**
     * @brief Generates a JSON object with six per-attribute stats.
     *
     * Returns a JSON object with keys: pace, shooting, passing, dribbling,
     * defending, physical.  Values are weighted so the position's primary
     * attributes receive higher ranges.
     *
     * @param position Position string: "Goalkeeper", "Defence", "Midfield", or "Attack".
     * @return nlohmann::json object with integer stats in [40, 99].
     */
    static nlohmann::json generateStats(const std::string& position);

private:
    /**
     * @brief Returns a uniformly distributed random integer in [min, max].
     * @param min Lower bound (inclusive).
     * @param max Upper bound (inclusive).
     * @return Random integer.
     */
    static int randomStat(int min, int max);
};