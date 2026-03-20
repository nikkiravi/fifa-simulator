// include/game/ChemistryCalculator.h
#pragma once
#include <array>
#include <memory>
#include <string>

// Forward declarations to avoid circular includes
class Card;
class Squad;

// ─────────────────────────────────────────────────────────────────────────────
// Strategy interface
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Abstract strategy interface for computing player chemistry (Strategy pattern).
 *
 * Concrete strategies define how individual player chemistry scores are
 * derived from a squad's composition.  New strategies can be added
 * without modifying ChemistryCalculator or any existing strategy.
 *
 * Concrete implementations:
 *  - FIFAChemistryStrategy   — position + nationality + club link bonuses
 *  - SimpleChemistryStrategy — position-only baseline (10 or 0)
 */
class ChemistryStrategy {
public:
    virtual ~ChemistryStrategy() = default;

    /**
     * @brief Calculates the chemistry score for one player slot.
     *
     * @param slotIndex        Index of the player slot being evaluated (0–10).
     * @param requiredPosition Position label the Formation expects for this slot.
     * @param allSlots         All 11 card pointers (nullptr = empty slot).
     * @return Integer chemistry score in [0, 10].
     */
    virtual int calculatePlayerChemistry(
        int slotIndex,
        const std::string& requiredPosition,
        const std::array<Card*, 11>& allSlots) const = 0;

    /**
     * @return Human-readable name of this strategy (used in reports).
     */
    virtual std::string getName() const = 0;
};

// ─────────────────────────────────────────────────────────────────────────────
// Concrete Strategy A: FIFA-style chemistry
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief FIFA-style chemistry strategy considering position, nationality, and club links.
 *
 * Individual player chemistry score (0–10) is built from three components:
 *  - **+3** if the player's position matches the formation slot.
 *  - **+1** per squadmate sharing the same nationality (capped at +3).
 *  - **+2** per squadmate sharing the same club (capped at +4).
 *
 * The total is clamped to [0, 10].
 *
 * **Team chemistry** = average of all 11 individual scores × 10 (max 100).
 */
class FIFAChemistryStrategy : public ChemistryStrategy {
public:
    /**
     * @brief Computes FIFA-style chemistry for the player in @p slotIndex.
     * @param slotIndex        Slot index (0–10) of the player being evaluated.
     * @param requiredPosition Position the formation requires for this slot.
     * @param allSlots         All 11 card pointers.
     * @return Chemistry score in [0, 10].
     */
    int calculatePlayerChemistry(
        int slotIndex,
        const std::string& requiredPosition,
        const std::array<Card*, 11>& allSlots) const override;

    /**
     * @return "FIFA Chemistry"
     */
    std::string getName() const override { return "FIFA Chemistry"; }
};

// ─────────────────────────────────────────────────────────────────────────────
// Concrete Strategy B: Simple (position-only) chemistry
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Simplified chemistry strategy that only considers player position.
 *
 * A player scores **10** if placed in the correct position slot, **0** otherwise.
 * This strategy ignores nationality and club links, making it a useful baseline
 * for comparing the impact of squad links in FIFAChemistryStrategy.
 */
class SimpleChemistryStrategy : public ChemistryStrategy {
public:
    /**
     * @brief Computes position-only chemistry for the player in @p slotIndex.
     * @param slotIndex        Slot index (0–10) of the player being evaluated.
     * @param requiredPosition Position the formation requires for this slot.
     * @param allSlots         All 11 card pointers.
     * @return 10 if the player's position matches @p requiredPosition; 0 otherwise.
     */
    int calculatePlayerChemistry(
        int slotIndex,
        const std::string& requiredPosition,
        const std::array<Card*, 11>& allSlots) const override;

    /**
     * @return "Simple (Position-Only) Chemistry"
     */
    std::string getName() const override { return "Simple (Position-Only) Chemistry"; }
};

// ─────────────────────────────────────────────────────────────────────────────
// Context class
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Context class that delegates chemistry calculation to a swappable strategy.
 *
 * ChemistryCalculator holds a unique_ptr<ChemistryStrategy> and exposes methods
 * to calculate individual and team chemistry scores.  The active strategy can be
 * replaced at runtime with setStrategy(), demonstrating the Strategy pattern.
 *
 * Example:
 * @code
 *   ChemistryCalculator calc(std::make_unique<FIFAChemistryStrategy>());
 *   calc.displayChemistry(mySquad);
 *
 *   // Swap strategy without changing the squad or the calculator
 *   calc.setStrategy(std::make_unique<SimpleChemistryStrategy>());
 *   calc.displayChemistry(mySquad);
 * @endcode
 */
class ChemistryCalculator {
private:
    std::unique_ptr<ChemistryStrategy> strategy_; ///< Active chemistry strategy.

public:
    /**
     * @brief Constructs the calculator with an initial strategy.
     * @param strategy The chemistry strategy to use initially.
     */
    explicit ChemistryCalculator(std::unique_ptr<ChemistryStrategy> strategy);

    /**
     * @brief Replaces the active strategy at runtime.
     * @param newStrategy The replacement strategy.
     */
    void setStrategy(std::unique_ptr<ChemistryStrategy> newStrategy);

    /**
     * @brief Computes chemistry scores for every slot in the squad.
     * @param squad The squad to evaluate.
     * @return Array of 11 chemistry scores (0–10 each), indexed by slot.
     */
    std::array<int, 11> calculateAllPlayerChemistries(const Squad& squad) const;

    /**
     * @brief Computes the overall team chemistry score.
     *
     * Team chemistry = (average individual score across filled slots) × 10,
     * clamped to [0, 100].
     *
     * @param squad The squad to evaluate.
     * @return Team chemistry score in [0, 100].
     */
    int calculateTeamChemistry(const Squad& squad) const;

    /**
     * @brief Prints a formatted chemistry report to stdout.
     *
     * The report shows each slot's player name, actual position, required
     * position, and individual chemistry score, followed by the overall team
     * chemistry score and a visual bar.
     *
     * @param squad The squad to report on.
     */
    void displayChemistry(const Squad& squad) const;
};