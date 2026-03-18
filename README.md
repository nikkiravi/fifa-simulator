# FIFA Ultimate Team Pack Simulator

A comprehensive C++ application that simulates the FIFA Ultimate Team pack opening experience with real player data, MongoDB integration, and advanced squad building features.

## Project Overview

This project is a semester-long C++ application that demonstrates:
- Object-Oriented Programming principles (inheritance, polymorphism, encapsulation)
- REST API integration with football-data.org
- MongoDB database operations
- Design patterns (Factory, Singleton, Strategy)
- Modern C++ features (C++17)

## Features

- **Player Class Hierarchy**: Base Player class with specialized GKPlayer, DefenderPlayer, MidfielderPlayer, and AttackerPlayer classes
- **API Integration**: Fetch real player data from football-data.org REST API
- **Database Caching**: Store player data in MongoDB for offline access
- **Pack Opening System**: Probability-based pack opening with Bronze, Silver, Gold, and Premium Gold tiers
- **Collection Management**: Search, filter, and sort your card collection
- **Squad Builder**: Build 11-player squads with different formations (4-3-3, 4-4-2, 3-5-2)
- **Chemistry System**: Calculate individual (0–10) and team (0–100) chemistry via Strategy pattern — swappable between FIFA-style (position + nation + club links) and simple (position-only) strategies

## Current Status: Milestone 6 ✓

**Completed:**
- ✓ Milestone 1: Player class hierarchy (base class + 4 derived classes)
- ✓ Milestone 2: HTTP client & API integration (CPR + football-data.org)
- ✓ Milestone 3: PlayerFetcher & MongoDB caching (100+ players with generated FIFA stats)
- ✓ Milestone 4: Pack system (Factory pattern), Card classes, Club collection manager
- ✓ Milestone 5: Formation & Squad classes, position validation, MongoDB persistence
- ✓ Milestone 6: Chemistry system (Strategy pattern), individual + team chemistry scores

## Technology Stack

| Technology | Purpose |
|------------|---------|
| C++17 | Core language |
| CMake 3.15+ | Build system |
| vcpkg | Package manager |
| CPR | HTTP client (football-data.org API) |
| nlohmann/json | JSON parsing |
| MongoDB + mongocxx | Database with C++ driver |
| Git | Version control |

## Project Structure

```
fifa-simulator/
├── include/              # Header files
│   ├── models/           # Player & Card class headers
│   ├── api/              # APIClient, PlayerFetcher headers
│   ├── packs/            # Pack system headers
│   ├── database/         # Database singleton header
│   ├── game/             # Club, Squad, Formation headers
│   └── utils/            # Config, RatingGenerator headers
├── src/                  # Implementation files (mirrors include/)
│   └── main.cpp          # Entry point
├── data/
│   └── formations.json   # Formation slot definitions
├── build/                # Build output (gitignored)
└── CMakeLists.txt        # CMake configuration
```

## Class Hierarchy

```
Player (Abstract)
├── GKPlayer
├── DefenderPlayer
├── MidfielderPlayer
└── AttackerPlayer

Card (Abstract, HAS-A Player)
├── BronzeCard
├── SilverCard
└── GoldCard

Pack (Abstract)
├── BronzePack       (5 cards)
├── SilverPack       (10 cards)
├── GoldPack         (10 cards)
└── PremiumGoldPack  (12 cards)

ChemistryStrategy (Abstract)
├── FIFAChemistryStrategy   (position + nation + club links)
└── SimpleChemistryStrategy (position-only baseline)

ChemistryCalculator (Context — holds a swappable ChemistryStrategy)
```

## Design Patterns

| Pattern | Where used |
|---------|-----------|
| Singleton | APIClient, Config, DatabaseClient |
| Factory | PackFactory |
| Composition | Card HAS-A Player |
| Strategy | Chemistry system (Milestone 6) |

## Build Instructions

### Prerequisites

- C++ compiler with C++17 support (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.15 or higher
- vcpkg with: `cpr`, `nlohmann-json`, `mongo-cxx-driver`
- MongoDB server running locally on port 27017
- `.env` file with `API_KEY` and `API_BASE_URL` (see football-data.org)

### Building

```bash
cd build
cmake ..
make
./fifa_simulator
```

## Development Timeline

- **Milestone 1** (Weeks 1-2): Project Setup & Player Classes ✓
- **Milestone 2** (Weeks 3-4): HTTP Client & API Integration ✓
- **Milestone 3** (Weeks 5-6): PlayerFetcher & Data Caching ✓
- **Milestone 4** (Weeks 7-8): Pack System & Collection ✓
- **Milestone 5** (Weeks 9-10): Squad Builder & Formations ✓
- **Milestone 6** (Weeks 11-12): Chemistry System ✓
- **Milestone 7** (Weeks 13-14): Polish & Documentation

## License

Educational project for CS 3XX: Advanced C++ Programming

## Author

Created as part of an advanced C++ programming course.