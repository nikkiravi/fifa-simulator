#include <iostream>
#include <memory>
#include "models/AttackerPlayer.h"
#include "utils/Config.h"
#include "api/APIClient.h"
#include "api/PlayerFetcher.h"
#include "database/Database.h"

int main() {
  try {
    // Test Milestone 1 - Player classes
  //   std::cout << "=== Testing Milestone 1 - Player Classes ===\n";
  //   auto messi = std::make_unique<AttackerPlayer>("Lionel Messi", "Argentina", "Barcelona", "LW", 91, 97, 88, 82, 92, 39, 78);
  //   messi->displayInfo();
  //   std::cout << "Milestone 1 Complete!\n\n";
  //
  //   // Test Milestone 2 - Config loading
  //   std::cout << "=== Testing Milestone 2 - Config Loading ===\n";
  //   Config& config = Config::getInstance();
  //
  //   std::cout << "API Key: " << config.get("API_KEY").substr(0, 10) << "..." << std::endl;
  //   std::cout << "API Base URL: " << config.get("API_BASE_URL") << std::endl;
  //   std::cout << "Config loading successful!\n\n";
  //
  //   // Test Milestone 2 - API Client
  //   std::cout << "=== Testing Milestone 2 - API Client ===\n";
  //   APIClient& client = APIClient::getInstance(
  //       config.get("API_KEY"),
  //       config.get("API_BASE_URL")
  //   );
  //
  //   // Fetch Premier League teams
  //   auto response = client.get("/competitions/PL/teams");
  //
  //   std::cout << "\nFetched Premier League Teams:\n";
  //   std::cout << "Total teams: " << response["count"] << "\n";
  //
  //   // Display first 3 teams
  //   int count = 0;
  //   for (const auto& team : response["teams"]) {
  //       if (count++ >= 3) break;
  //       std::cout << "  - " << team["name"] << " (" << team["tla"] << ")\n";
  //   }
  //
  //   std::cout << "\nMilestone 2 - API Integration Complete!\n";

    // Test Milestone 3 - Player fetching and caching
    std::cout << "=== Testing Milestone 3 - Player Fetching & Caching ===\n";

    Config& config = Config::getInstance();

    APIClient& client = APIClient::getInstance(
        config.get("API_KEY"),
        config.get("API_BASE_URL")
    );

    DatabaseClient& db = DatabaseClient::getInstance();

    PlayerFetcher fetcher(client, db);
    fetcher.fetchAndCachePlayers(100);

    std::cout << "Milestone 3 Complete!\n";

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}