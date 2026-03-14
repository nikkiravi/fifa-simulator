#pragma once
#include "models/Player.h"
#include <string>
#include <iostream>

class AttackerPlayer : public Player { // inheriting from Player class
  public:
    AttackerPlayer(const std::string& name, const std::string& nationality, const std::string& club, const std::string& position, int overall_rating, int pace, int shooting, int passing, int dribbling, int defending, int physicality);
    void displayInfo() const override;
};