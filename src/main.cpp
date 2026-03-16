#include <iostream>
#include <memory>
#include "models/AttackerPlayer.h"
#include "utils/Config.h"
#include "api/APIClient.h"
#include "api/PlayerFetcher.h"
#include "database/Database.h"
#include "packs/PackFactory.h"
#include "game/Club.h"

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

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}