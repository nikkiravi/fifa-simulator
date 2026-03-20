# FIFA Ultimate Team Pack Simulator — Design Document

## Project Description

This project is a C++ simulation of the FIFA Ultimate Team experience. Users open virtual packs to collect player cards drawn from a real-world player database (fetched from the football-data.org REST API and cached in MongoDB). Collected cards can be organized into squads using standard football formations, and a chemistry system scores how well the squad's players complement each other based on shared position, nationality, and club.

The project was built across seven milestones to progressively introduce core C++ and software-engineering concepts: class hierarchies, REST APIs, database integration, design patterns (Factory, Singleton, Strategy), and interactive CLI design.

---

## Technology Stack

| Technology | Purpose |
|------------|---------|
| C++17 | Core language |
| CMake 3.15+ | Build system |
| vcpkg | Package manager |
| CPR | HTTP client for the football-data.org API |
| nlohmann/json | JSON parsing for API responses |
| MongoDB + mongocxx | Persistent storage for players, cards, and squads |
| Git | Version control |

---

## Setup & Prerequisites

1. **C++ compiler** with C++17 support (GCC 7+, Clang 5+, or MSVC 2017+)
2. **CMake** 3.15 or higher
3. **vcpkg** with the following packages installed:
   ```
   vcpkg install cpr nlohmann-json mongo-cxx-driver
   ```
4. **MongoDB** running locally on `mongodb://localhost:27017`
5. **`.env` file** in the project root containing your API credentials:
   ```
   API_KEY=your_football_data_org_key
   API_BASE_URL=https://api.football-data.org/v4
   ```
   Register for a free key at [football-data.org](https://www.football-data.org).

---

## Build & Run

```bash
# From the project root
mkdir build && cd build
cmake ..
make
./fifa_simulator
```

The executable launches an interactive text-based menu. No command-line arguments are needed.

---

## First-Time Database Population

The player database must be seeded **once** before pack opening will work. To do this, temporarily add the following block inside `main()` in `src/main.cpp`, build and run once, then remove it:

```cpp
Config& config = Config::getInstance();
APIClient& client = APIClient::getInstance(config.get("API_KEY"), config.get("API_BASE_URL"));
DatabaseClient& db = DatabaseClient::getInstance();
PlayerFetcher fetcher(client, db);
fetcher.fetchAndCachePlayers(100);
```

This fetches 100+ real players from the API, generates FIFA-style stats for each, and stores them in the `players` collection in MongoDB. All subsequent runs use the cached data.

---

## File-by-File Description

### Entry Point

| File | Purpose |
|------|---------|
| `src/main.cpp` | Creates the `DatabaseClient` singleton and launches the `Menu` event loop. No game logic lives here. |

---

### Models — `include/models/` · `src/models/`

These files define the player and card class hierarchies.

| File | Purpose |
|------|---------|
| `Player.h / Player.cpp` | Abstract base class for all player types. Stores the seven core attributes (overall rating, pace, shooting, passing, dribbling, defending, physicality) plus biographical data (name, nationality, club, position). Declares the pure-virtual `displayInfo()`. |
| `GKPlayer.h / .cpp` | Concrete `Player` subclass for goalkeepers. `displayInfo()` emphasises pace and defending. |
| `DefenderPlayer.h / .cpp` | Concrete `Player` subclass for defenders. `displayInfo()` emphasises defending and physicality. |
| `MidfielderPlayer.h / .cpp` | Concrete `Player` subclass for midfielders. `displayInfo()` emphasises passing and dribbling. |
| `AttackerPlayer.h / .cpp` | Concrete `Player` subclass for attackers. `displayInfo()` emphasises pace and shooting. |
| `Card.h / Card.cpp` | Abstract base class for player cards (composition: Card HAS-A Player via `shared_ptr`). Adds a `CardRarity` (BRONZE / SILVER / GOLD) and declares the pure-virtual `displayCard()`. |
| `BronzeCard.h / .cpp` | Concrete `Card` for the bronze tier (OVR 55–64). `displayCard()` renders a bronze-styled summary. |
| `SilverCard.h / .cpp` | Concrete `Card` for the silver tier (OVR 65–74). |
| `GoldCard.h / .cpp` | Concrete `Card` for the gold tier (OVR 75–99). |

---

### Packs — `include/packs/` · `src/packs/`

These files implement the pack-opening system using the **Factory pattern**.

| File | Purpose |
|------|---------|
| `Pack.h / Pack.cpp` | Abstract base class for all pack types. Provides the protected helper `getRandomCards()`, which queries MongoDB for players in a rating band, shuffles them, and wraps them in the correct `Card` subclass. |
| `BronzePack.h / .cpp` | Opens 5 bronze cards. |
| `SilverPack.h / .cpp` | Opens 3 silver + 2 bronze cards. |
| `GoldPack.h / .cpp` | Opens 2 gold + 2 silver + 1 bronze card. |
| `PremiumGoldPack.h / .cpp` | Opens 3 gold + 2 silver cards (no bronze). |
| `PackFactory.h / PackFactory.cpp` | Factory that maps a `PackType` enum (or name string) to the correct `Pack` subclass. Callers never need to include concrete pack headers directly. |

---

### Game Logic — `include/game/` · `src/game/`

These files implement the high-level gameplay: collection management, squad building, formation rules, chemistry, and the interactive menu.

| File | Purpose |
|------|---------|
| `Club.h / Club.cpp` | Manages the user's card collection. Stores cards as `unique_ptr<Card>` in memory and persists each card to the `user_cards` MongoDB collection. Provides search (by name, position, club, nationality), filter (by rarity, rating range), sort (by rating, by position), and display methods. |
| `Formation.h / Formation.cpp` | Represents a tactical formation. Stores an array of 11 position strings (e.g., `["Goalkeeper","Defence","Defence",...]`) that define what position is required in each squad slot. Supports 4-3-3, 4-4-2, and 3-5-2. |
| `Squad.h / Squad.cpp` | Manages an 11-player squad. Enforces positional constraints when adding cards (a card's position must match the formation slot). Handles serialisation to and deserialisation from the `squads` MongoDB collection. |
| `ChemistryCalculator.h / ChemistryCalculator.cpp` | Implements the **Strategy pattern** for chemistry scoring. The `ChemistryCalculator` context holds a `ChemistryStrategy` and delegates calculation to it. `FIFAChemistryStrategy` awards points for correct position (+3), shared nationality (up to +3), and shared club (up to +4). `SimpleChemistryStrategy` gives 10 for a correct position, 0 otherwise. Team chemistry = average individual score × 10. |
| `Menu.h / Menu.cpp` | Top-level interactive CLI controller. Owns a `Club` and exposes four sub-menus: Pack Store, My Collection, Squad Builder, and Chemistry Lab. Input is validated with `readInt()` / `readLine()` helpers. The main loop runs until the user selects Exit. |

---

### API — `include/api/` · `src/api/`

| File | Purpose |
|------|---------|
| `APIClient.h / APIClient.cpp` | **Singleton** HTTP client built on CPR. Sends authenticated GET requests to the football-data.org v4 API using the `X-Auth-Token` header. Returns parsed `nlohmann::json` objects. |
| `PlayerFetcher.h / PlayerFetcher.cpp` | Coordinates `APIClient`, `RatingGenerator`, and `DatabaseClient` to fetch player rosters from the API, generate FIFA-style stats, and insert new players into MongoDB (skipping duplicates). |

---

### Database — `include/database/` · `src/database/`

| File | Purpose |
|------|---------|
| `Database.h / Database.cpp` | **Singleton** MongoDB client. Manages three collections: `players` (master roster), `user_cards` (opened cards), and `squads` (saved squads). Exposes typed CRUD methods for each collection and three plain data structs (`PlayerData`, `UserCardData`, `SquadData`) for transferring data between layers. |

---

### Utilities — `include/utils/` · `src/utils/`

| File | Purpose |
|------|---------|
| `Config.h / Config.cpp` | **Singleton** that reads a `.env` file (KEY=VALUE format) and exposes values via `get(key)`. Used to load the API key and base URL at startup. |
| `RatingGenerator.h / RatingGenerator.cpp` | Static utility that synthesises realistic FIFA-style overall ratings and per-attribute stats for a given position. Used by `PlayerFetcher` because the football-data.org API does not provide player ratings directly. |

---

## Design Patterns Summary

| Pattern | Files |
|---------|-------|
| **Singleton** | `APIClient`, `Config`, `DatabaseClient` — one shared instance for the lifetime of the program |
| **Factory** | `PackFactory` — creates the correct `Pack` subclass from a `PackType` enum |
| **Composition** | `Card` HAS-A `Player` via `shared_ptr` |
| **Strategy** | `ChemistryCalculator` + `FIFAChemistryStrategy` / `SimpleChemistryStrategy` — algorithm swappable at runtime |
| **Inheritance / Polymorphism** | `Player` → 4 subclasses; `Card` → 3 subclasses; `Pack` → 4 subclasses |

---

## MongoDB Collections

| Collection | Contents |
|------------|---------|
| `players` | Master player roster (name, nationality, club, position, rating, stats). Populated once by `PlayerFetcher`. |
| `user_cards` | Every card the user has obtained by opening packs. Each document mirrors a `Card` + `Player`. |
| `squads` | Saved squad configurations, each storing the formation name and an array of 11 player slots. |

---

## Interactive Menu Overview

```
=======================================================
      FIFA Ultimate Team Pack Simulator
=======================================================
  Collection: 37 card(s)
-------------------------------------------------------
  1. Pack Store        — open packs, add cards to collection
  2. My Collection     — view, sort, filter, search cards
  3. Squad Builder     — build & save 11-player squads
  4. Chemistry Lab     — run chemistry reports on saved squads
  0. Exit
```

---