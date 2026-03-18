// src/game/ChemistryCalculator.cpp
#include "game/ChemistryCalculator.h"
#include "game/Squad.h"
#include "models/Card.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

// ─── helpers ─────────────────────────────────────────────────────────────────

// Map a Player position string ("Goalkeeper", "Defence", …) to the
// short label used when printing the chemistry table.
static std::string shortPos(const std::string& pos) {
    if (pos == "Goalkeeper") return "GK ";
    if (pos == "Defence")    return "DEF";
    if (pos == "Midfield")   return "MID";
    if (pos == "Attack")     return "ATT";
    return pos.substr(0, 3);
}

// ─── FIFAChemistryStrategy ───────────────────────────────────────────────────

int FIFAChemistryStrategy::calculatePlayerChemistry(
    int slotIndex,
    const std::string& requiredPosition,
    const std::array<Card*, 11>& allSlots) const
{
    Card* card = allSlots[slotIndex];
    if (!card) return 0;

    const auto& player = card->getPlayer();
    int chem = 0;

    // +3 for correct position
    if (player->getPosition() == requiredPosition) {
        chem += 3;
    }

    // Links from other players in the squad
    int nationBonus = 0;
    int clubBonus   = 0;

    for (int i = 0; i < 11; ++i) {
        if (i == slotIndex || !allSlots[i]) continue;
        const auto& other = allSlots[i]->getPlayer();

        if (other->getNationality() == player->getNationality()) {
            nationBonus += 1;
        }
        if (other->getClub() == player->getClub()) {
            clubBonus += 2;
        }
    }

    // Cap each link type so a single abundant link can't dominate
    chem += std::min(nationBonus, 3); // nationality contributes at most +3
    chem += std::min(clubBonus,   4); // club        contributes at most +4

    return std::min(chem, 10);
}

// ─── SimpleChemistryStrategy ─────────────────────────────────────────────────

int SimpleChemistryStrategy::calculatePlayerChemistry(
    int slotIndex,
    const std::string& requiredPosition,
    const std::array<Card*, 11>& allSlots) const
{
    Card* card = allSlots[slotIndex];
    if (!card) return 0;
    return (card->getPlayer()->getPosition() == requiredPosition) ? 10 : 0;
}

// ─── ChemistryCalculator ─────────────────────────────────────────────────────

ChemistryCalculator::ChemistryCalculator(std::unique_ptr<ChemistryStrategy> strategy)
    : strategy_(std::move(strategy)) {}

void ChemistryCalculator::setStrategy(std::unique_ptr<ChemistryStrategy> newStrategy) {
    strategy_ = std::move(newStrategy);
}

std::array<int, 11> ChemistryCalculator::calculateAllPlayerChemistries(
    const Squad& squad) const
{
    std::array<int, 11> chemistries{};
    std::array<Card*, 11> allSlots{};
    for (int i = 0; i < 11; ++i) allSlots[i] = squad.getSlot(i);

    for (int i = 0; i < 11; ++i) {
        const std::string required = squad.getFormation().getSlotPosition(i);
        chemistries[i] = strategy_->calculatePlayerChemistry(i, required, allSlots);
    }
    return chemistries;
}

int ChemistryCalculator::calculateTeamChemistry(const Squad& squad) const {
    auto chemistries = calculateAllPlayerChemistries(squad);
    int total = 0;
    int filled = 0;
    for (int i = 0; i < 11; ++i) {
        if (squad.getSlot(i)) {
            total += chemistries[i];
            ++filled;
        }
    }
    if (filled == 0) return 0;
    // Team chemistry = average individual chemistry × 10, capped at 100
    int teamChem = (total * 10) / filled;
    return std::min(teamChem, 100);
}

void ChemistryCalculator::displayChemistry(const Squad& squad) const {
    auto chemistries = calculateAllPlayerChemistries(squad);

    std::cout << "\n=== Chemistry Report ===\n";
    std::cout << "Strategy : " << strategy_->getName() << "\n";
    std::cout << "Squad    : " << squad.getName() << "\n";
    std::cout << "Formation: " << squad.getFormation().getName() << "\n\n";

    // Header
    std::cout << std::left
              << std::setw(5)  << "Slot"
              << std::setw(22) << "Player"
              << std::setw(6)  << "Pos"
              << std::setw(6)  << "Req'd"
              << "Chem\n";
    std::cout << std::string(50, '-') << "\n";

    for (int i = 0; i < 11; ++i) {
        Card* card = squad.getSlot(i);
        const std::string required = squad.getFormation().getSlotPosition(i);

        std::cout << std::setw(5) << i;

        if (!card) {
            std::cout << std::setw(22) << "(empty)"
                      << std::setw(6)  << "---"
                      << std::setw(6)  << shortPos(required)
                      << "  --/10\n";
        } else {
            const auto& p = card->getPlayer();
            std::string name = p->getName();
            if (name.size() > 20) name = name.substr(0, 19) + ".";

            // Mark with ✓ if in correct position, ✗ otherwise
            std::string posMatch = (p->getPosition() == required) ? "[OK]" : "[!!]";

            std::cout << std::setw(22) << name
                      << std::setw(6)  << shortPos(p->getPosition())
                      << std::setw(6)  << shortPos(required)
                      << "  " << chemistries[i] << "/10 " << posMatch << "\n";
        }
    }

    int teamChem = calculateTeamChemistry(squad);
    std::cout << std::string(50, '-') << "\n";
    std::cout << "Team Chemistry: " << teamChem << " / 100\n";

    // Visual bar
    int barLen = teamChem / 5; // 0-20 chars
    std::cout << "[";
    for (int i = 0; i < 20; ++i) std::cout << (i < barLen ? '#' : '.');
    std::cout << "] " << teamChem << "%\n\n";
}