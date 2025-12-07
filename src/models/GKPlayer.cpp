#include "models/GKPlayer.h"

GKPlayer::GKPlayer(const std::string &name, const std::string &nationality,
                   const std::string &club, const std::string &position,
                   int overall_rating, int pacing, int shooting, int passing,
                   int dribbling, int defending, int physicality) : Player(name, nationality, club, position, overall_rating, pacing, shooting, passing, dribbling, defending, physicality) {};

void GKPlayer::displayInfo() const {
  std::cout << name << " (" << position << ") - " << overall_rating << " OVR\n";
  std::cout << " Pace: " << pace << " | Defending: " << defending << "\n";
}

