// include/game/Formation.h
#pragma once
#include <string>
#include <array>

enum class FormationType {
    FOUR_THREE_THREE,
    FOUR_FOUR_TWO,
    THREE_FIVE_TWO
};

// Formation defines the structure of a squad: a display name and 11 position slots.
// Each slot holds the required player position for that spot.
class Formation {
private:
    FormationType type;
    std::string name;
    std::array<std::string, 11> slots; // required position label per slot

public:
    explicit Formation(FormationType type);

    FormationType getType() const { return type; }
    std::string getName() const { return name; }
    const std::array<std::string, 11>& getSlots() const { return slots; }
    std::string getSlotPosition(int index) const { return slots[index]; }
};