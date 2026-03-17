#include <iostream>
#include <memory>
#include <map>
#include "models/AttackerPlayer.h"
#include "utils/Config.h"
#include "api/APIClient.h"
#include "api/PlayerFetcher.h"
#include "database/Database.h"
#include "packs/PackFactory.h"
#include "game/Club.h"
#include "game/Formation.h"
#include "game/Squad.h"

int main() {
  try {
    // ── Milestone 3 (commented out once DB is populated) ──────────────────
    // std::cout << "=== Milestone 3 - Player Fetching & Caching ===\n";
    // Config& config = Config::getInstance();
    // APIClient& client = APIClient::getInstance(
    //     config.get("API_KEY"), config.get("API_BASE_URL"));
    // DatabaseClient& db = DatabaseClient::getInstance();
    // PlayerFetcher fetcher(client, db);
    // fetcher.fetchAndCachePlayers(100);
    // std::cout << "Milestone 3 Complete!\n";

    // ── Milestone 4 - Pack System & Collection ────────────────────────────
    std::cout << "=== Testing Milestone 4 - Pack System & Collection ===\n\n";

    DatabaseClient& db = DatabaseClient::getInstance();
    Club myClub(db);

    // 1. Open one of each pack type using the Factory
    std::cout << "--- Opening packs ---\n";

    auto bronzePack = PackFactory::createPack(PackType::BRONZE);
    std::cout << "\nOpening: " << bronzePack->getPackName() << "\n";
    std::cout << bronzePack->getPackDescription() << "\n";
    auto bronzeCards = bronzePack->open();
    for (const auto& c : bronzeCards) c->displayCard();
    myClub.addCards(std::move(bronzeCards));

    auto silverPack = PackFactory::createPack(PackType::SILVER);
    std::cout << "\nOpening: " << silverPack->getPackName() << "\n";
    std::cout << silverPack->getPackDescription() << "\n";
    auto silverCards = silverPack->open();
    for (const auto& c : silverCards) c->displayCard();
    myClub.addCards(std::move(silverCards));

    auto goldPack = PackFactory::createPack(PackType::GOLD);
    std::cout << "\nOpening: " << goldPack->getPackName() << "\n";
    std::cout << goldPack->getPackDescription() << "\n";
    auto goldCards = goldPack->open();
    for (const auto& c : goldCards) c->displayCard();
    myClub.addCards(std::move(goldCards));

    auto premiumPack = PackFactory::createPack(PackType::PREMIUM_GOLD);
    std::cout << "\nOpening: " << premiumPack->getPackName() << "\n";
    std::cout << premiumPack->getPackDescription() << "\n";
    auto premiumCards = premiumPack->open();
    for (const auto& c : premiumCards) c->displayCard();
    myClub.addCards(std::move(premiumCards));

    // 2. Display collection statistics
    myClub.displayStats();

    // 3. Sort by rating and show full collection
    myClub.sortByRating();
    myClub.displayCollection();

    // 4. Filter: show only Gold cards
    std::cout << "\n--- Gold cards in collection ---\n";
    auto goldOnly = myClub.filterByRarity(CardRarity::GOLD);
    for (Card* c : goldOnly) c->displayCard();
    std::cout << "Total gold: " << goldOnly.size() << "\n";

    // 5. Filter: cards rated 80+
    std::cout << "\n--- Cards rated 80 or above ---\n";
    auto highRated = myClub.filterByRating(80, 99);
    for (Card* c : highRated) c->displayCard();

    // 6. Persistence check: reload from DB
    std::cout << "\n--- Reloading collection from MongoDB ---\n";
    Club reloadedClub(db);
    reloadedClub.loadFromDatabase();
    reloadedClub.displayStats();

    std::cout << "\nMilestone 4 Complete!\n";

    // ── Milestone 5 - Squad Builder & Formations ──────────────────────────────
    std::cout << "\n=== Testing Milestone 5 - Squad Builder & Formations ===\n\n";

    // 1. Show all three available formations
    std::cout << "--- Available Formations ---\n";
    Formation f433(FormationType::FOUR_THREE_THREE);
    Formation f442(FormationType::FOUR_FOUR_TWO);
    Formation f352(FormationType::THREE_FIVE_TWO);
    std::cout << "  " << f433.getName() << "  slots: ";
    for (const auto& s : f433.getSlots()) std::cout << s[0]; // first letter shorthand
    std::cout << "\n  " << f442.getName() << "  slots: ";
    for (const auto& s : f442.getSlots()) std::cout << s[0];
    std::cout << "\n  " << f352.getName() << "  slots: ";
    for (const auto& s : f352.getSlots()) std::cout << s[0];
    std::cout << "\n\n";

    // 2. Build a 4-3-3 squad using cards from the collection
    std::cout << "--- Building a 4-3-3 squad ---\n";
    Squad mySquad("My First Squad", FormationType::FOUR_THREE_THREE);

    // Reload the club so we have cards in memory
    Club squadClub(db);
    squadClub.loadFromDatabase();

    // Helper: pick the first unused card of a given position
    auto pickByPosition = [&](const std::string& pos) -> Card* {
        auto results = squadClub.searchByPosition(pos);
        return results.empty() ? nullptr : results[0];
    };

    // Slot layout for 4-3-3:
    //   0=GK, 1-4=DEF, 5-7=MID, 8-10=ATT
    struct SlotFill { int slot; std::string pos; };
    std::vector<SlotFill> plan = {
        {0,  "Goalkeeper"},
        {1,  "Defence"},  {2,  "Defence"}, {3,  "Defence"}, {4,  "Defence"},
        {5,  "Midfield"}, {6,  "Midfield"}, {7,  "Midfield"},
        {8,  "Attack"},   {9,  "Attack"},   {10, "Attack"}
    };

    // Track how many of each position we've already assigned
    std::map<std::string, int> posCount;
    for (const auto& sf : plan) {
        auto cards = squadClub.searchByPosition(sf.pos);
        int idx = posCount[sf.pos]++;
        if (idx < static_cast<int>(cards.size())) {
            bool ok = mySquad.addPlayer(sf.slot, cards[idx]);
            if (!ok) std::cout << "  (skipped slot " << sf.slot << " - position mismatch)\n";
        } else {
            std::cout << "  Not enough " << sf.pos << " cards in collection for slot "
                      << sf.slot << "\n";
        }
    }

    // 3. Display the squad
    mySquad.displaySquad();

    // 4. Demonstrate position validation (intentionally wrong)
    std::cout << "\n--- Position validation demo ---\n";
    auto attackers = squadClub.searchByPosition("Attack");
    if (!attackers.empty()) {
        std::cout << "Trying to put an Attacker into slot 0 (Goalkeeper):\n  ";
        bool ok = mySquad.addPlayer(0, attackers[0]);
        if (!ok) std::cout << "  Correctly rejected!\n";
    }

    // 5. Save to MongoDB
    std::cout << "\n--- Saving squad to MongoDB ---\n";
    mySquad.saveToDatabase(db);

    // 6. List saved squads
    auto squadNames = db.getAllSquadNames();
    std::cout << "Saved squads in DB: " << squadNames.size() << "\n";
    for (const auto& n : squadNames) std::cout << "  - " << n << "\n";

    // 7. Reload from MongoDB and display
    std::cout << "\n--- Reloading squad from MongoDB ---\n";
    Squad reloadedSquad = Squad::loadFromDatabase("My First Squad", db);
    reloadedSquad.displaySquad();

    std::cout << "\nMilestone 5 Complete!\n";

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}