#include "models/AttackerPlayer.h"

AttackPlayer::AttackPlayer(const std::string &name,
                           const std::string &nationality,
                           const std::string &club, const std::string &position,
                           int overall_rating, int pace, int shooting,
                           int passing, int dribbling, int defending,
                           int physicality) : Player(name, nationality, club, position, overall_rating, pace, shooting, passing, dribbling, defending, physicality) {};

void AttackPlayer::displayInfo() const {
  std::cout << name << " (" << position << ") - " << overall_rating << " OVR\n";
  std::cout << " Pace: " << pace << " | Shooting: " << shooting << "\n";
}
