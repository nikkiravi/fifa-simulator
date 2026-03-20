// include/game/Menu.h
#pragma once
#include <string>
#include <set>
#include "game/Club.h"
#include "game/Squad.h"
#include "game/ChemistryCalculator.h"
#include "database/Database.h"

/**
 * @brief Interactive text-based menu system for the FIFA Ultimate Team Simulator.
 *
 * Menu is the top-level controller for the application's user interface.
 * It owns a Club (the user's card collection) and exposes four sub-menus:
 *
 *  - **Pack Store**         — open Bronze / Silver / Gold / Premium Gold packs
 *  - **Collection Manager** — view, sort, filter, and search cards
 *  - **Squad Builder**      — interactively fill an 11-player squad and save it
 *  - **Chemistry Lab**      — run FIFA or Simple chemistry reports on saved squads
 *
 * Typical usage:
 * @code
 *   DatabaseClient& db = DatabaseClient::getInstance();
 *   Menu menu(db);
 *   menu.run();   // blocks until the user chooses Exit
 * @endcode
 */
class Menu {
public:
    /**
     * @brief Constructs the menu and connects it to the active database.
     * @param db Reference to the singleton DatabaseClient.
     */
    explicit Menu(DatabaseClient& db);

    /**
     * @brief Starts the main event loop; returns only when the user exits.
     *
     * Loads the existing card collection from MongoDB on startup, then
     * repeatedly shows the main menu until the user selects option 0 (Exit).
     */
    void run();

private:
    DatabaseClient& db_;  ///< Non-owning reference to the database singleton.
    Club club_;           ///< The user's in-memory card collection.

    // ── Sub-menus ─────────────────────────────────────────────────────────────

    /**
     * @brief Pack Store sub-menu.
     *
     * Lets the user choose a pack type, opens it, displays the cards, and
     * adds them to the collection (both in-memory and in MongoDB).
     */
    void packStore();

    /**
     * @brief Collection Manager sub-menu.
     *
     * Provides options to view all cards, sort, filter by rarity or rating
     * range, and search by name, position, club, or nationality.
     */
    void collectionManager();

    /**
     * @brief Squad Builder sub-menu.
     *
     * Allows the user to build a new squad interactively (slot by slot),
     * or load and display a previously saved squad from MongoDB.
     */
    void squadBuilder();

    /**
     * @brief Chemistry Lab sub-menu.
     *
     * Loads a saved squad and runs the FIFA chemistry strategy, the Simple
     * (position-only) strategy, or a side-by-side comparison of both.
     */
    void chemistryLab();

    // ── Helpers ───────────────────────────────────────────────────────────────

    /**
     * @brief Guides the user through placing 11 players into a new squad.
     *
     * Asks the user to pick a formation and squad name, then iterates over
     * each slot and shows available cards of the required position.
     *
     * @return The filled (or partially filled) Squad object.
     *         The squad is not saved to the database by this method.
     */
    Squad interactiveBuildSquad();

    /**
     * @brief Reads and validates an integer in [lo, hi] from stdin.
     *
     * Re-prompts the user on invalid input until a valid value is entered.
     *
     * @param prompt Text to display before reading.
     * @param lo     Minimum acceptable value (inclusive).
     * @param hi     Maximum acceptable value (inclusive).
     * @return Validated integer entered by the user.
     */
    static int readInt(const std::string& prompt, int lo, int hi);

    /**
     * @brief Reads a full line of text from stdin.
     * @param prompt Text to display before reading.
     * @return The line entered by the user (may be empty if the user just presses Enter).
     */
    static std::string readLine(const std::string& prompt);

    /**
     * @brief Prints a "Press Enter to continue..." message and waits.
     *
     * Used after displaying results to give the user time to read output
     * before the menu is redrawn.
     */
    static void pause();

    /**
     * @brief Prints a horizontal separator line to stdout.
     * @param c Character to repeat (default: '-').
     * @param w Total width in characters (default: 55).
     */
    static void separator(char c = '-', int w = 55);
};