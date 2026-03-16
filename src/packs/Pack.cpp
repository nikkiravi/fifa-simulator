// src/packs/Pack.cpp
#include "packs/Pack.h"
#include "models/AttackerPlayer.h"
#include "models/DefenderPlayer.h"
#include "models/MidfielderPlayer.h"
#include "models/GKPlayer.h"
#include "models/BronzeCard.h"
#include "models/SilverCard.h"
#include "models/GoldCard.h"
#include <algorithm>
#include <random>
#include <iostream>

std::shared_ptr<Player> Pack::buildPlayer(const PlayerData& pd) {
    if (pd.position == "Goalkeeper") {
        return std::make_shared<GKPlayer>(pd.name, pd.nationality, pd.club, pd.position,
                                          pd.rating, pd.pace, pd.shooting, pd.passing,
                                          pd.dribbling, pd.defending, pd.physical);
    } else if (pd.position == "Defence") {
        return std::make_shared<DefenderPlayer>(pd.name, pd.nationality, pd.club, pd.position,
                                                pd.rating, pd.pace, pd.shooting, pd.passing,
                                                pd.dribbling, pd.defending, pd.physical);
    } else if (pd.position == "Midfield") {
        return std::make_shared<MidfielderPlayer>(pd.name, pd.nationality, pd.club, pd.position,
                                                  pd.rating, pd.pace, pd.shooting, pd.passing,
                                                  pd.dribbling, pd.defending, pd.physical);
    } else {
        return std::make_shared<AttackerPlayer>(pd.name, pd.nationality, pd.club, pd.position,
                                                pd.rating, pd.pace, pd.shooting, pd.passing,
                                                pd.dribbling, pd.defending, pd.physical);
    }
}

std::vector<std::unique_ptr<Card>> Pack::getRandomCards(CardRarity rarity, int count) {
    // Card rarity rating thresholds
    int minRating, maxRating;
    if (rarity == CardRarity::BRONZE) {
        minRating = 0;
        maxRating = 74;
    } else if (rarity == CardRarity::SILVER) {
        minRating = 75;
        maxRating = 82;
    } else { // GOLD
        minRating = 83;
        maxRating = 99;
    }

    DatabaseClient& db = DatabaseClient::getInstance();
    auto players = db.getPlayerDataByRatingRange(minRating, maxRating);

    // Shuffle to randomise selection
    static std::mt19937 rng(std::random_device{}());
    std::shuffle(players.begin(), players.end(), rng);

    std::vector<std::unique_ptr<Card>> cards;
    for (int i = 0; i < count && i < static_cast<int>(players.size()); i++) {
        auto player = buildPlayer(players[i]);
        if (rarity == CardRarity::BRONZE) {
            cards.push_back(std::make_unique<BronzeCard>(player));
        } else if (rarity == CardRarity::SILVER) {
            cards.push_back(std::make_unique<SilverCard>(player));
        } else {
            cards.push_back(std::make_unique<GoldCard>(player));
        }
    }

    if (static_cast<int>(cards.size()) < count) {
        std::cerr << "Warning: only " << cards.size() << " of " << count
                  << " requested " << (rarity == CardRarity::BRONZE ? "bronze"
                                      : rarity == CardRarity::SILVER ? "silver" : "gold")
                  << " cards found in DB.\n";
    }

    return cards;
}