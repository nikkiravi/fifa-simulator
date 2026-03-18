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
// Each concrete strategy defines how individual player chemistry is computed.
// ─────────────────────────────────────────────────────────────────────────────
class ChemistryStrategy {
public:
    virtual ~ChemistryStrategy() = default;

    // Calculate chemistry for the player in slots[slotIndex].
    // requiredPosition : the position label the formation expects for that slot.
    // allSlots         : all 11 card pointers (nullptr = empty slot).
    // Returns an integer in [0, 10].
    virtual int calculatePlayerChemistry(
        int slotIndex,
        const std::string& requiredPosition,
        const std::array<Card*, 11>& allSlots) const = 0;

    virtual std::string getName() const = 0;
};

// ─────────────────────────────────────────────────────────────────────────────
// Concrete Strategy A: FIFA-style chemistry
//
// Individual player chemistry (0–10):
//   +3  if the player's position matches the formation slot
//   +1  per squadmate who shares the same nationality  (capped at 3 bonuses)
//   +2  per squadmate who shares the same club         (capped at 2 bonuses)
//   Total is clamped to [0, 10].
//
// Team chemistry = average of all 11 individual scores × 10  (max 100).
// ─────────────────────────────────────────────────────────────────────────────
class FIFAChemistryStrategy : public ChemistryStrategy {
public:
    int calculatePlayerChemistry(
        int slotIndex,
        const std::string& requiredPosition,
        const std::array<Card*, 11>& allSlots) const override;

    std::string getName() const override { return "FIFA Chemistry"; }
};

// ─────────────────────────────────────────────────────────────────────────────
// Concrete Strategy B: Simple (position-only) chemistry
//
// Individual player chemistry:
//   10 if the player is in the correct position slot, 0 otherwise.
//
// Useful as a baseline / teaching comparison.
// ─────────────────────────────────────────────────────────────────────────────
class SimpleChemistryStrategy : public ChemistryStrategy {
public:
    int calculatePlayerChemistry(
        int slotIndex,
        const std::string& requiredPosition,
        const std::array<Card*, 11>& allSlots) const override;

    std::string getName() const override { return "Simple (Position-Only) Chemistry"; }
};

// ─────────────────────────────────────────────────────────────────────────────
// Context class — holds a strategy and delegates all calculations to it.
// The strategy can be swapped at runtime (setStrategy).
// ─────────────────────────────────────────────────────────────────────────────
class ChemistryCalculator {
private:
    std::unique_ptr<ChemistryStrategy> strategy_;

public:
    explicit ChemistryCalculator(std::unique_ptr<ChemistryStrategy> strategy);

    // Swap the active strategy at runtime.
    void setStrategy(std::unique_ptr<ChemistryStrategy> newStrategy);

    // Per-player chemistry scores for every slot [0–10].
    std::array<int, 11> calculateAllPlayerChemistries(const Squad& squad) const;

    // Team chemistry score [0–100].
    int calculateTeamChemistry(const Squad& squad) const;

    // Print a formatted chemistry report to stdout.
    void displayChemistry(const Squad& squad) const;
};