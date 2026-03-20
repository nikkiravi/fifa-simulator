// src/game/Menu.cpp
#include "game/Menu.h"
#include "packs/PackFactory.h"
#include "game/Formation.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <string>

// ── Construction ──────────────────────────────────────────────────────────────

Menu::Menu(DatabaseClient& db)
    : db_(db), club_(db) {}

// ── Static utilities ──────────────────────────────────────────────────────────

int Menu::readInt(const std::string& prompt, int lo, int hi) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= lo && value <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Please enter a number between " << lo
                  << " and " << hi << ".\n";
    }
}

std::string Menu::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void Menu::pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Menu::separator(char c, int w) {
    std::cout << std::string(w, c) << "\n";
}

// ── Main loop ─────────────────────────────────────────────────────────────────

void Menu::run() {
    std::cout << "Loading your collection from database...\n";
    club_.loadFromDatabase();

    while (true) {
        std::cout << "\n";
        separator('=');
        std::cout << "      FIFA Ultimate Team Pack Simulator\n";
        separator('=');
        std::cout << "  Collection: " << club_.getCardCount() << " card(s)\n";
        separator();
        std::cout << "  1. Pack Store\n";
        std::cout << "  2. My Collection\n";
        std::cout << "  3. Squad Builder\n";
        std::cout << "  4. Chemistry Lab\n";
        std::cout << "  0. Exit\n";
        separator();

        int choice = readInt("Select option: ", 0, 4);
        switch (choice) {
            case 1: packStore();         break;
            case 2: collectionManager(); break;
            case 3: squadBuilder();      break;
            case 4: chemistryLab();      break;
            case 0:
                std::cout << "\nThanks for playing! Goodbye.\n";
                return;
        }
    }
}

// ── Pack Store ────────────────────────────────────────────────────────────────

void Menu::packStore() {
    while (true) {
        std::cout << "\n";
        separator('=');
        std::cout << "                   Pack Store\n";
        separator('=');
        std::cout << "  1. Bronze Pack         ( 5 cards | OVR 55-64)\n";
        std::cout << "  2. Silver Pack         (10 cards | OVR 65-74)\n";
        std::cout << "  3. Gold Pack           (10 cards | OVR 75-99)\n";
        std::cout << "  4. Premium Gold Pack   (12 cards | OVR 75-99)\n";
        std::cout << "  0. Back\n";
        separator();

        int choice = readInt("Select pack: ", 0, 4);
        if (choice == 0) return;

        PackType type;
        switch (choice) {
            case 1: type = PackType::BRONZE;       break;
            case 2: type = PackType::SILVER;       break;
            case 3: type = PackType::GOLD;         break;
            case 4: type = PackType::PREMIUM_GOLD; break;
            default: continue;
        }

        auto pack = PackFactory::createPack(type);
        std::cout << "\n  Opening: " << pack->getPackName() << "...\n";
        std::cout << "  " << pack->getPackDescription() << "\n\n";
        separator();

        auto cards = pack->open();
        for (const auto& c : cards) {
            c->displayCard();
        }
        separator();

        club_.addCards(std::move(cards));
        pause();
    }
}

// ── Collection Manager ────────────────────────────────────────────────────────

void Menu::collectionManager() {
    while (true) {
        std::cout << "\n";
        separator('=');
        std::cout << "        My Collection ("
                  << club_.getCardCount() << " cards)\n";
        separator('=');
        std::cout << "  1. View All Cards (sorted by rating)\n";
        std::cout << "  2. Sort & View by Position\n";
        std::cout << "  3. Filter by Rarity\n";
        std::cout << "  4. Filter by Rating Range\n";
        std::cout << "  5. Search by Name\n";
        std::cout << "  6. Search by Position\n";
        std::cout << "  7. Search by Club\n";
        std::cout << "  8. Search by Nationality\n";
        std::cout << "  9. Collection Stats\n";
        std::cout << "  0. Back\n";
        separator();

        int choice = readInt("Select option: ", 0, 9);
        if (choice == 0) return;

        switch (choice) {
            case 1:
                club_.sortByRating();
                club_.displayCollection();
                break;

            case 2:
                club_.sortByPosition();
                club_.displayCollection();
                break;

            case 3: {
                std::cout << "  Rarity:  1=Bronze   2=Silver   3=Gold\n";
                int r = readInt("  Select rarity: ", 1, 3);
                CardRarity rarity = (r == 1) ? CardRarity::BRONZE
                                  : (r == 2) ? CardRarity::SILVER
                                             : CardRarity::GOLD;
                auto results = club_.filterByRarity(rarity);
                std::cout << "\nFound " << results.size() << " card(s):\n";
                for (Card* c : results) c->displayCard();
                break;
            }

            case 4: {
                int lo = readInt("  Minimum rating (55-99): ", 55, 99);
                int hi = readInt("  Maximum rating ("
                                 + std::to_string(lo) + "-99): ", lo, 99);
                auto results = club_.filterByRating(lo, hi);
                std::cout << "\nFound " << results.size() << " card(s):\n";
                for (Card* c : results) c->displayCard();
                break;
            }

            case 5: {
                std::string name = readLine("  Player name: ");
                auto results = club_.searchByName(name);
                std::cout << "\nFound " << results.size() << " card(s):\n";
                for (Card* c : results) c->displayCard();
                break;
            }

            case 6: {
                std::cout << "  Positions: Goalkeeper | Defence | Midfield | Attack\n";
                std::string pos = readLine("  Position: ");
                auto results = club_.searchByPosition(pos);
                std::cout << "\nFound " << results.size() << " card(s):\n";
                for (Card* c : results) c->displayCard();
                break;
            }

            case 7: {
                std::string clubName = readLine("  Club name: ");
                auto results = club_.searchByClub(clubName);
                std::cout << "\nFound " << results.size() << " card(s):\n";
                for (Card* c : results) c->displayCard();
                break;
            }

            case 8: {
                std::string nat = readLine("  Nationality: ");
                auto results = club_.searchByNationality(nat);
                std::cout << "\nFound " << results.size() << " card(s):\n";
                for (Card* c : results) c->displayCard();
                break;
            }

            case 9:
                club_.displayStats();
                break;
        }
        pause();
    }
}

// ── Squad Builder ─────────────────────────────────────────────────────────────

void Menu::squadBuilder() {
    while (true) {
        auto savedNames = db_.getAllSquadNames();

        std::cout << "\n";
        separator('=');
        std::cout << "                Squad Builder\n";
        separator('=');

        if (savedNames.empty()) {
            std::cout << "  No saved squads yet.\n";
        } else {
            std::cout << "  Saved squads (" << savedNames.size() << "):\n";
            for (const auto& n : savedNames) {
                std::cout << "    - " << n << "\n";
            }
        }

        separator();
        std::cout << "  1. Build New Squad\n";
        std::cout << "  2. Load & Display a Saved Squad\n";
        std::cout << "  0. Back\n";
        separator();

        int choice = readInt("Select option: ", 0, 2);
        if (choice == 0) return;

        if (choice == 1) {
            Squad built = interactiveBuildSquad();
            built.displaySquad();

            int save = readInt("\nSave this squad to the database? (1=Yes  0=No): ", 0, 1);
            if (save == 1) {
                built.saveToDatabase(db_);
            }
            pause();
        } else {
            auto names = db_.getAllSquadNames();
            if (names.empty()) {
                std::cout << "  No saved squads found. Build one first.\n";
                pause();
                continue;
            }

            std::cout << "\n  Select a squad:\n";
            for (int i = 0; i < (int)names.size(); ++i) {
                std::cout << "  " << (i + 1) << ". " << names[i] << "\n";
            }
            std::cout << "  0. Cancel\n";

            int pick = readInt("  Squad: ", 0, (int)names.size());
            if (pick == 0) continue;

            Squad loaded = Squad::loadFromDatabase(names[pick - 1], db_);
            loaded.displaySquad();
            pause();
        }
    }
}

Squad Menu::interactiveBuildSquad() {
    separator('=');
    std::cout << "              Build New Squad\n";
    separator('=');

    // 1. Pick formation
    std::cout << "  Select formation:\n";
    std::cout << "    1. 4-3-3  (4 Def, 3 Mid, 3 Att)\n";
    std::cout << "    2. 4-4-2  (4 Def, 4 Mid, 2 Att)\n";
    std::cout << "    3. 3-5-2  (3 Def, 5 Mid, 2 Att)\n";
    int fChoice = readInt("  Formation: ", 1, 3);

    FormationType ft;
    if      (fChoice == 1) ft = FormationType::FOUR_THREE_THREE;
    else if (fChoice == 2) ft = FormationType::FOUR_FOUR_TWO;
    else                   ft = FormationType::THREE_FIVE_TWO;

    // 2. Squad name
    std::string squadName = readLine("  Squad name: ");
    if (squadName.empty()) squadName = "My Squad";

    // 3. Reload collection so we have up-to-date cards (includes any just opened)
    club_.loadFromDatabase();

    Squad squad(squadName, ft);
    Formation formation(ft);
    std::set<Card*> usedCards; // tracks cards already placed in this squad

    std::cout << "\n  Filling slots for " << formation.getName() << "...\n";

    for (int i = 0; i < 11; ++i) {
        const std::string required = formation.getSlotPosition(i);

        // Build list of available (unused) cards for this position
        auto all = club_.searchByPosition(required);
        std::vector<Card*> available;
        for (Card* c : all) {
            if (!usedCards.count(c)) available.push_back(c);
        }

        separator();
        std::cout << "  Slot " << i << "  [" << required << "]\n";

        if (available.empty()) {
            std::cout << "  No available " << required
                      << " cards — slot left empty.\n";
            continue;
        }

        // Print table header
        std::cout << std::left
                  << std::setw(4)  << "#"
                  << std::setw(24) << "Name"
                  << std::setw(6)  << "OVR"
                  << std::setw(9)  << "Rarity"
                  << "Club\n";
        separator('-', 55);

        for (int j = 0; j < (int)available.size(); ++j) {
            auto p = available[j]->getPlayer();
            std::string name = p->getName();
            if (name.size() > 22) name = name.substr(0, 21) + ".";
            std::cout << std::left
                      << std::setw(4)  << (j + 1)
                      << std::setw(24) << name
                      << std::setw(6)  << p->getOverallRating()
                      << std::setw(9)  << available[j]->getRarityString()
                      << p->getClub() << "\n";
        }
        std::cout << "  0. Leave empty\n";

        int pick = readInt("  Pick: ", 0, (int)available.size());
        if (pick > 0) {
            Card* chosen = available[pick - 1];
            squad.addPlayer(i, chosen);
            usedCards.insert(chosen);
            std::cout << "  Added: " << chosen->getPlayer()->getName() << "\n";
        }
    }

    return squad;
}

// ── Chemistry Lab ─────────────────────────────────────────────────────────────

void Menu::chemistryLab() {
    while (true) {
        std::cout << "\n";
        separator('=');
        std::cout << "               Chemistry Lab\n";
        separator('=');
        std::cout << "  1. FIFA Chemistry Report    (position + nation + club)\n";
        std::cout << "  2. Simple Chemistry Report  (position-only)\n";
        std::cout << "  3. Compare Both Strategies\n";
        std::cout << "  0. Back\n";
        separator();

        int choice = readInt("Select option: ", 0, 3);
        if (choice == 0) return;

        auto names = db_.getAllSquadNames();
        if (names.empty()) {
            std::cout << "  No saved squads found. Build and save a squad first.\n";
            pause();
            continue;
        }

        // Let the user pick a squad
        std::cout << "\n  Select a squad:\n";
        for (int i = 0; i < (int)names.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << names[i] << "\n";
        }
        std::cout << "  0. Cancel\n";

        int pick = readInt("  Squad: ", 0, (int)names.size());
        if (pick == 0) continue;

        Squad squad = Squad::loadFromDatabase(names[pick - 1], db_);

        if (choice == 1) {
            ChemistryCalculator calc(std::make_unique<FIFAChemistryStrategy>());
            calc.displayChemistry(squad);
        } else if (choice == 2) {
            ChemistryCalculator calc(std::make_unique<SimpleChemistryStrategy>());
            calc.displayChemistry(squad);
        } else {
            // Side-by-side comparison: run both strategies in sequence
            ChemistryCalculator calc(std::make_unique<FIFAChemistryStrategy>());
            calc.displayChemistry(squad);
            std::cout << "--- Switching to Simple (position-only) strategy ---\n";
            calc.setStrategy(std::make_unique<SimpleChemistryStrategy>());
            calc.displayChemistry(squad);
        }

        pause();
    }
}