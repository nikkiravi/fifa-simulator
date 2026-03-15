#include "utils/RatingGenerator.h"
#include <random>

int RatingGenerator::randomStat(int min, int max) {
    static std::mt19937 rng(std::random_device{}());
    return std::uniform_int_distribution<int>(min, max)(rng);
}

nlohmann::json RatingGenerator::generateStats(const std::string& position) {
    nlohmann::json stats;
    if (position == "Goalkeeper") {
        stats["pace"]      = randomStat(40, 60);
        stats["shooting"]  = randomStat(10, 30);
        stats["passing"]   = randomStat(50, 70);
        stats["dribbling"] = randomStat(30, 50);
        stats["defending"] = randomStat(60, 80);
        stats["physical"]  = randomStat(60, 80);
    } else if (position == "Defence") {
        stats["pace"]      = randomStat(50, 78);
        stats["shooting"]  = randomStat(20, 45);
        stats["passing"]   = randomStat(55, 75);
        stats["dribbling"] = randomStat(40, 65);
        stats["defending"] = randomStat(70, 90);
        stats["physical"]  = randomStat(65, 85);
    } else if (position == "Midfield") {
        stats["pace"]      = randomStat(60, 82);
        stats["shooting"]  = randomStat(50, 75);
        stats["passing"]   = randomStat(70, 90);
        stats["dribbling"] = randomStat(65, 85);
        stats["defending"] = randomStat(45, 70);
        stats["physical"]  = randomStat(58, 78);
    } else { // Offence / Unknown
        stats["pace"]      = randomStat(70, 95);
        stats["shooting"]  = randomStat(70, 92);
        stats["passing"]   = randomStat(60, 82);
        stats["dribbling"] = randomStat(70, 92);
        stats["defending"] = randomStat(20, 45);
        stats["physical"]  = randomStat(55, 78);
    }
    return stats;
}

int RatingGenerator::generateRating(const std::string& position) {
    if (position == "Goalkeeper") return randomStat(68, 88);
    if (position == "Defence")    return randomStat(70, 89);
    if (position == "Midfield")   return randomStat(72, 90);
    return randomStat(73, 92); // Offence
}