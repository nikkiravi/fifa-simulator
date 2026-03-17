// src/game/Formation.cpp
#include "game/Formation.h"
#include <stdexcept>

// Populate name and the 11 position slots based on the chosen formation.
// Positions match the strings stored in MongoDB: "Goalkeeper", "Defence",
// "Midfield", "Attack".
Formation::Formation(FormationType type) : type(type) {
    switch (type) {
        case FormationType::FOUR_THREE_THREE:
            name  = "4-3-3";
            slots = { "Goalkeeper",
                      "Defence", "Defence", "Defence", "Defence",
                      "Midfield", "Midfield", "Midfield",
                      "Attack", "Attack", "Attack" };
            break;

        case FormationType::FOUR_FOUR_TWO:
            name  = "4-4-2";
            slots = { "Goalkeeper",
                      "Defence", "Defence", "Defence", "Defence",
                      "Midfield", "Midfield", "Midfield", "Midfield",
                      "Attack", "Attack" };
            break;

        case FormationType::THREE_FIVE_TWO:
            name  = "3-5-2";
            slots = { "Goalkeeper",
                      "Defence", "Defence", "Defence",
                      "Midfield", "Midfield", "Midfield", "Midfield", "Midfield",
                      "Attack", "Attack" };
            break;

        default:
            throw std::invalid_argument("Unknown FormationType");
    }
}