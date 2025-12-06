#include "models/Player.h"

Player::Player(const std::string &name, const std::string &nationality,
               const std::string &club, const std::string &position, int overall_rating,
               int pace, int shooting, int passing, int dribbling,
               int defending, int physicality) : name(name), nationality(nationality), club(club), position(position),
                                                 overall_rating(overall_rating), pace(pace), shooting(shooting),
                                                 passing(passing), dribbling(dribbling), defending(defending),
                                                 physicality(physicality) {
}
