// src/game/Squad.cpp
#include "game/Squad.h"
#include "models/AttackerPlayer.h"
#include "models/DefenderPlayer.h"
#include "models/MidfielderPlayer.h"
#include "models/GKPlayer.h"
#include "models/BronzeCard.h"
#include "models/SilverCard.h"
#include "models/GoldCard.h"
#include <bsoncxx/v_noabi/bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/v_noabi/bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/v_noabi/bsoncxx/builder/basic/kvp.hpp>
#include <iostream>
#include <stdexcept>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

Squad::Squad(const std::string& name, FormationType formationType)
    : name(name), formation(formationType) {
    slots.fill(nullptr);
}

// ── private helpers ──────────────────────────────────────────────────────────

std::unique_ptr<Card> Squad::cardFromData(const UserCardData& d) const {
    std::shared_ptr<Player> player;

    if (d.position == "Goalkeeper") {
        player = std::make_shared<GKPlayer>(d.name, d.nationality, d.club, d.position,
                                            d.rating, d.pace, d.shooting, d.passing,
                                            d.dribbling, d.defending, d.physical);
    } else if (d.position == "Defence") {
        player = std::make_shared<DefenderPlayer>(d.name, d.nationality, d.club, d.position,
                                                  d.rating, d.pace, d.shooting, d.passing,
                                                  d.dribbling, d.defending, d.physical);
    } else if (d.position == "Midfield") {
        player = std::make_shared<MidfielderPlayer>(d.name, d.nationality, d.club, d.position,
                                                    d.rating, d.pace, d.shooting, d.passing,
                                                    d.dribbling, d.defending, d.physical);
    } else {
        player = std::make_shared<AttackerPlayer>(d.name, d.nationality, d.club, d.position,
                                                  d.rating, d.pace, d.shooting, d.passing,
                                                  d.dribbling, d.defending, d.physical);
    }

    if (d.rarity == "Bronze") return std::make_unique<BronzeCard>(player);
    if (d.rarity == "Silver") return std::make_unique<SilverCard>(player);
    return std::make_unique<GoldCard>(player);
}

bsoncxx::v_noabi::document::value Squad::toDocument() const {
    auto playersArr = bsoncxx::builder::basic::array{};
    const auto& fslots = formation.getSlots();

    for (int i = 0; i < 11; ++i) {
        if (slots[i]) {
            // Filled slot: store all player & card data
            auto p = slots[i]->getPlayer();
            playersArr.append(make_document(
                kvp("slot",        static_cast<int32_t>(i)),
                kvp("name",        p->getName()),
                kvp("nationality", p->getNationality()),
                kvp("club",        p->getClub()),
                kvp("position",    p->getPosition()),
                kvp("rating",      static_cast<int32_t>(p->getOverallRating())),
                kvp("rarity",      slots[i]->getRarityString()),
                kvp("stats", make_document(
                    kvp("pace",      static_cast<int32_t>(p->getPace())),
                    kvp("shooting",  static_cast<int32_t>(p->getShooting())),
                    kvp("passing",   static_cast<int32_t>(p->getPassing())),
                    kvp("dribbling", static_cast<int32_t>(p->getDribbling())),
                    kvp("defending", static_cast<int32_t>(p->getDefending())),
                    kvp("physical",  static_cast<int32_t>(p->getPhysicality()))
                ))
            ));
        } else {
            // Empty slot: placeholder so we keep slot indices intact on reload
            playersArr.append(make_document(
                kvp("slot",     static_cast<int32_t>(i)),
                kvp("name",     std::string("")),
                kvp("position", fslots[i])
            ));
        }
    }

    return make_document(
        kvp("name",      name),
        kvp("formation", formation.getName()),
        kvp("players",   playersArr)
    );
}

// ── public interface ─────────────────────────────────────────────────────────

bool Squad::addPlayer(int slotIndex, Card* card) {
    if (slotIndex < 0 || slotIndex >= 11) {
        std::cerr << "addPlayer: invalid slot index " << slotIndex << "\n";
        return false;
    }
    const std::string& required = formation.getSlotPosition(slotIndex);
    const std::string& actual   = card->getPlayer()->getPosition();
    if (actual != required) {
        std::cerr << "Position mismatch at slot " << slotIndex
                  << ": formation requires \"" << required
                  << "\" but card is \"" << actual << "\"\n";
        return false;
    }
    slots[slotIndex] = card;
    return true;
}

void Squad::removePlayer(int slotIndex) {
    if (slotIndex >= 0 && slotIndex < 11) {
        slots[slotIndex] = nullptr;
    }
}

int Squad::calculateOverallRating() const {
    int total = 0, count = 0;
    for (int i = 0; i < 11; ++i) {
        if (slots[i]) {
            total += slots[i]->getPlayer()->getOverallRating();
            ++count;
        }
    }
    return count > 0 ? total / count : 0;
}

bool Squad::isComplete() const {
    for (int i = 0; i < 11; ++i) {
        if (!slots[i]) return false;
    }
    return true;
}

void Squad::saveToDatabase(DatabaseClient& db) const {
    db.insertSquad(toDocument());
    std::cout << "Squad \"" << name << "\" saved to MongoDB.\n";
}

Squad Squad::loadFromDatabase(const std::string& squadName, DatabaseClient& db) {
    SquadData data = db.getSquadByName(squadName);

    // Map formation string back to enum
    FormationType ftype;
    if      (data.formation == "4-3-3") ftype = FormationType::FOUR_THREE_THREE;
    else if (data.formation == "4-4-2") ftype = FormationType::FOUR_FOUR_TWO;
    else                                ftype = FormationType::THREE_FIVE_TWO;

    Squad squad(data.squadName, ftype);

    for (int i = 0; i < static_cast<int>(data.players.size()); ++i) {
        const UserCardData& ucd = data.players[i];
        if (ucd.name.empty()) continue; // empty slot placeholder
        auto card = squad.cardFromData(ucd);
        squad.slots[i] = card.get();
        squad.ownedCards.push_back(std::move(card));
    }

    std::cout << "Squad \"" << squadName << "\" loaded from MongoDB.\n";
    return squad;
}

void Squad::displaySquad() const {
    const auto& fslots = formation.getSlots();
    std::cout << "\n=== Squad: " << name << "  [" << formation.getName() << "] ===\n";
    for (int i = 0; i < 11; ++i) {
        std::cout << "  Slot " << i << "  [" << fslots[i] << "]:  ";
        if (slots[i]) {
            auto p = slots[i]->getPlayer();
            std::cout << p->getName()
                      << "  (" << p->getPosition() << ")  "
                      << p->getOverallRating() << " OVR"
                      << "  " << slots[i]->getRarityString() << "\n";
        } else {
            std::cout << "--- empty ---\n";
        }
    }
    std::cout << "Overall Rating : " << calculateOverallRating() << "\n";
    std::cout << "Complete       : " << (isComplete() ? "Yes" : "No") << "\n";
}