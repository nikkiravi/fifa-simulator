// src/game/Club.cpp
#include "game/Club.h"
#include "models/AttackerPlayer.h"
#include "models/DefenderPlayer.h"
#include "models/MidfielderPlayer.h"
#include "models/GKPlayer.h"
#include "models/BronzeCard.h"
#include "models/SilverCard.h"
#include "models/GoldCard.h"
#include <bsoncxx/v_noabi/bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/v_noabi/bsoncxx/builder/basic/kvp.hpp>
#include <algorithm>
#include <iostream>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

Club::Club(DatabaseClient& db) : db(db) {}

// ── helpers ─────────────────────────────────────────────────────────────────

std::unique_ptr<Card> Club::cardFromData(const UserCardData& d) const {
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

bsoncxx::v_noabi::document::value Club::cardToDocument(const Card& card) const {
    auto p = card.getPlayer();
    auto stats_doc = make_document(
        kvp("pace",      static_cast<int32_t>(p->getPace())),
        kvp("shooting",  static_cast<int32_t>(p->getShooting())),
        kvp("passing",   static_cast<int32_t>(p->getPassing())),
        kvp("dribbling", static_cast<int32_t>(p->getDribbling())),
        kvp("defending", static_cast<int32_t>(p->getDefending())),
        kvp("physical",  static_cast<int32_t>(p->getPhysicality()))
    );
    return make_document(
        kvp("name",        p->getName()),
        kvp("nationality", p->getNationality()),
        kvp("club",        p->getClub()),
        kvp("position",    p->getPosition()),
        kvp("rating",      static_cast<int32_t>(p->getOverallRating())),
        kvp("rarity",      card.getRarityString()),
        kvp("stats",       stats_doc)
    );
}

// ── public interface ─────────────────────────────────────────────────────────

void Club::addCards(std::vector<std::unique_ptr<Card>> cards) {
    for (auto& card : cards) {
        auto doc = cardToDocument(*card);
        db.insertUserCard(doc);
        collection.push_back(std::move(card));
    }
    std::cout << "Added " << cards.size() << " card(s) to your collection.\n";
}

void Club::loadFromDatabase() {
    collection.clear();
    auto data = db.getUserCardData();
    for (const auto& d : data) {
        collection.push_back(cardFromData(d));
    }
    std::cout << "Loaded " << collection.size() << " card(s) from database.\n";
}

// ── search ───────────────────────────────────────────────────────────────────

std::vector<Card*> Club::searchByName(const std::string& name) const {
    std::vector<Card*> results;
    std::string query = name;
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);
    for (const auto& card : collection) {
        std::string pname = card->getPlayer()->getName();
        std::transform(pname.begin(), pname.end(), pname.begin(), ::tolower);
        if (pname.find(query) != std::string::npos) {
            results.push_back(card.get());
        }
    }
    return results;
}

std::vector<Card*> Club::searchByPosition(const std::string& position) const {
    std::vector<Card*> results;
    for (const auto& card : collection) {
        if (card->getPlayer()->getPosition() == position) {
            results.push_back(card.get());
        }
    }
    return results;
}

std::vector<Card*> Club::searchByClub(const std::string& clubName) const {
    std::vector<Card*> results;
    for (const auto& card : collection) {
        if (card->getPlayer()->getClub() == clubName) {
            results.push_back(card.get());
        }
    }
    return results;
}

std::vector<Card*> Club::searchByNationality(const std::string& nationality) const {
    std::vector<Card*> results;
    for (const auto& card : collection) {
        if (card->getPlayer()->getNationality() == nationality) {
            results.push_back(card.get());
        }
    }
    return results;
}

// ── filter ───────────────────────────────────────────────────────────────────

std::vector<Card*> Club::filterByRating(int minRating, int maxRating) const {
    std::vector<Card*> results;
    for (const auto& card : collection) {
        int r = card->getPlayer()->getOverallRating();
        if (r >= minRating && r <= maxRating) {
            results.push_back(card.get());
        }
    }
    return results;
}

std::vector<Card*> Club::filterByRarity(CardRarity rarity) const {
    std::vector<Card*> results;
    for (const auto& card : collection) {
        if (card->getRarity() == rarity) {
            results.push_back(card.get());
        }
    }
    return results;
}

// ── sort ─────────────────────────────────────────────────────────────────────

void Club::sortByRating() {
    std::sort(collection.begin(), collection.end(), [](const auto& a, const auto& b) {
        return a->getPlayer()->getOverallRating() > b->getPlayer()->getOverallRating();
    });
}

void Club::sortByPosition() {
    std::sort(collection.begin(), collection.end(), [](const auto& a, const auto& b) {
        return a->getPlayer()->getPosition() < b->getPlayer()->getPosition();
    });
}

// ── display ──────────────────────────────────────────────────────────────────

void Club::displayCollection() const {
    if (collection.empty()) {
        std::cout << "Your collection is empty.\n";
        return;
    }
    std::cout << "\n=== Your Collection (" << collection.size() << " cards) ===\n";
    for (const auto& card : collection) {
        card->displayCard();
    }
}

void Club::displayStats() const {
    int bronze = 0, silver = 0, gold = 0;
    int totalRating = 0, highestRating = 0;

    for (const auto& card : collection) {
        int r = card->getPlayer()->getOverallRating();
        totalRating += r;
        if (r > highestRating) highestRating = r;
        switch (card->getRarity()) {
            case CardRarity::BRONZE: bronze++; break;
            case CardRarity::SILVER: silver++; break;
            case CardRarity::GOLD:   gold++;   break;
        }
    }

    std::cout << "\n=== Club Statistics ===\n";
    std::cout << "Total Cards : " << collection.size() << "\n";
    std::cout << "  Bronze    : " << bronze << "\n";
    std::cout << "  Silver    : " << silver << "\n";
    std::cout << "  Gold      : " << gold   << "\n";
    if (!collection.empty()) {
        std::cout << "Avg Rating  : " << totalRating / static_cast<int>(collection.size()) << "\n";
        std::cout << "Best Rating : " << highestRating << "\n";
    }
}