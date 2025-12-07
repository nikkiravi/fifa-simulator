#pragma once
#include "models/Player.h"
#include <string>
#include <iostream>

class GKPlayer : public Player {
public:
  GKPlayer(const std::string& name, const std::string& nationality, const std::string& club, const std::string& position, int overall_rating, int pacing, int shooting, int passing, int dribbling, int defending, int physicality);
  void displayInfo() const override;
};