// src/main.cpp
//
// Milestone 7 — Polish & Documentation
//
// Entry point for the FIFA Ultimate Team Pack Simulator.
// All application logic is handled by the Menu class, which provides
// an interactive text-based interface for every feature built across
// milestones 1–6 (player hierarchy, API, database, packs, squads,
// and chemistry).
//
// To populate the player database for the first time, uncomment the
// Milestone 3 block in the history below and run once with MongoDB running.

#include <iostream>
#include "database/Database.h"
#include "game/Menu.h"

int main() {
    try {
        DatabaseClient& db = DatabaseClient::getInstance();
        Menu menu(db);
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}