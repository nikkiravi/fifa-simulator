#pragma once
#include <string>
#include <nlohmann/json.hpp>

class RatingGenerator {
public:
    static int generateRating(const std::string& position);
    static nlohmann::json generateStats(const std::string& position);
private:
    static int randomStat(int min, int max);
};