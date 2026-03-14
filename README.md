# FIFA Ultimate Team Pack Simulator

A comprehensive C++ application that simulates the FIFA Ultimate Team pack opening experience with real player data, MongoDB integration, and advanced squad building features.

## Project Overview

This project is a semester-long C++ application that demonstrates:
- Object-Oriented Programming principles (inheritance, polymorphism, encapsulation)
- REST API integration with FUT.GG
- MongoDB database operations
- Design patterns (Factory, Singleton, Strategy)
- Modern C++ features (C++17)

## Features (Planned)

- **Player Class Hierarchy**: Base Player class with specialized GKPlayer, DefenderPlayer, MidfielderPlayer, and AttackerPlayer classes
- **API Integration**: Fetch real FIFA player data from FUT.GG REST API
- **Database Caching**: Store player data in MongoDB for offline access
- **Pack Opening System**: Probability-based pack opening with different rarity tiers (Bronze, Silver, Gold)
- **Collection Management**: Search, filter, and sort your card collection
- **Squad Builder**: Build 11-player squads with different formations (4-3-3, 4-4-2, 3-5-2)
- **Chemistry System**: Calculate individual and team chemistry based on player links

## Current Status: Milestone 1 ✓

**Completed:**
- ✓ Project structure setup
- ✓ Player class hierarchy (base class + 4 derived classes)
- ✓ CMake build configuration
- ✓ Git version control
- ✓ Working inheritance demonstration

## Technology Stack

- **Language**: C++17
- **Build System**: CMake
- **Package Manager**: vcpkg
- **Database**: MongoDB (planned for Milestone 3)
- **HTTP Client**: CPR library (planned for Milestone 2)
- **JSON Parser**: nlohmann/json (planned for Milestone 2)
- **Version Control**: Git

## Project Structure

```
fifa-simulator/
├── include/              # Header files
│   ├── models/          # Player class headers
│   ├── api/             # API client headers (future)
│   ├── packs/           # Pack system headers (future)
│   ├── database/        # Database headers (future)
│   ├── game/            # Squad & chemistry headers (future)
│   └── utils/           # Utility headers (future)
├── src/                 # Implementation files
│   ├── models/          # Player class implementations
│   ├── api/             # API implementations (future)
│   ├── packs/           # Pack implementations (future)
│   ├── database/        # Database implementations (future)
│   ├── game/            # Game logic implementations (future)
│   ├── utils/           # Utility implementations (future)
│   └── main.cpp         # Entry point
├── data/                # Configuration files
├── scripts/             # Helper scripts
├── build/               # Build output (gitignored)
└── CMakeLists.txt       # CMake configuration
```

## Build Instructions

### Prerequisites

- C++ compiler with C++17 support (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.15 or higher

### Building the Project

```bash
# Navigate to project directory
cd fifa-simulator

# Create build directory and navigate to it
cd build

# Generate build files
cmake ..

# Compile the project
make

# Run the executable
./fifa_simulator
```

## Usage

Currently (Milestone 1), the application demonstrates the Player class hierarchy:

```bash
./fifa_simulator
```

This will create an AttackerPlayer instance (Lionel Messi) and display player information.

## Development Timeline

- **Milestone 1** (Weeks 1-2): Project Setup & Player Classes ✓
- **Milestone 2** (Weeks 3-4): HTTP Client & API Integration
- **Milestone 3** (Weeks 5-6): PlayerFetcher & Data Caching
- **Milestone 4** (Weeks 7-8): Pack System & Collection
- **Milestone 5** (Weeks 9-10): Squad Builder & Formations
- **Milestone 6** (Weeks 11-12): Chemistry System
- **Milestone 7** (Weeks 13-14): Polish & Documentation

## Class Hierarchy

```
Player (Abstract Base Class)
├── GKPlayer (Goalkeeper)
├── DefenderPlayer (Defenders)
├── MidfielderPlayer (Midfielders)
└── AttackerPlayer (Forwards)
```

## License

Educational project for CS 3XX: Advanced C++ Programming

## Author

Created as part of an advanced C++ programming course.