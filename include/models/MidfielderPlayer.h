#pragma once
#include "Player.h"
#include <string>
#include <iostream>

class MidfielderPlayer : public Player {
public:
  MidfielderPlayer(const std::string& name, const std::string& nationality, const std::string& club, const std::string& position, int overall_rating, int pace, int shooting, int passing, int dribbling, int defending, int physicality);
  void displayInfo() const override;
};