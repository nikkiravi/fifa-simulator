// include/game/Formation.h
#pragma once
#include <string>
#include <array>

/**
 * @brief Identifies one of the three supported tactical formations.
 */
enum class FormationType {
    FOUR_THREE_THREE, ///< 4-3-3: 4 defenders, 3 midfielders, 3 attackers.
    FOUR_FOUR_TWO,    ///< 4-4-2: 4 defenders, 4 midfielders, 2 attackers.
    THREE_FIVE_TWO    ///< 3-5-2: 3 defenders, 5 midfielders, 2 attackers.
};

/**
 * @brief Defines the structure of a squad: a display name and 11 position slots.
 *
 * A Formation holds an array of 11 position strings (one per player slot).
 * Each string is the required position label for that slot — e.g.,
 * "Goalkeeper", "Defence", "Midfield", or "Attack".
 *
 * Squad::addPlayer() uses these labels to validate that a card's position
 * matches the slot it is placed into.
 *
 * Supported formations and their slot layouts:
 * @code
 *   FOUR_THREE_THREE: [GK, DEF, DEF, DEF, DEF, MID, MID, MID, ATT, ATT, ATT]
 *   FOUR_FOUR_TWO:    [GK, DEF, DEF, DEF, DEF, MID, MID, MID, MID, ATT, ATT]
 *   THREE_FIVE_TWO:   [GK, DEF, DEF, DEF, MID, MID, MID, MID, MID, ATT, ATT]
 * @endcode
 */
class Formation {
private:
    FormationType type;              ///< Enum identifier for this formation.
    std::string name;                ///< Human-readable name (e.g., "4-3-3").
    std::array<std::string, 11> slots; ///< Required position label for each slot.

public:
    /**
     * @brief Constructs a Formation and populates its slot layout.
     * @param type The desired formation type.
     */
    explicit Formation(FormationType type);

    /**
     * @return The FormationType enum value for this formation.
     */
    FormationType getType() const { return type; }

    /**
     * @return Human-readable formation name (e.g., "4-3-3").
     */
    std::string getName() const { return name; }

    /**
     * @return Read-only reference to the 11-element slot array.
     */
    const std::array<std::string, 11>& getSlots() const { return slots; }

    /**
     * @brief Returns the required position label for a specific slot.
     * @param index Slot index in [0, 10].
     * @return Position string for that slot (e.g., "Defence").
     */
    std::string getSlotPosition(int index) const { return slots[index]; }
};