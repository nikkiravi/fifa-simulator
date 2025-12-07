//include/models/Player.h
#pragma once
#include <string>
#include <iostream>

class Player{
    protected:
        std::string name;
        std::string nationality;
        std::string club;
        std::string position;

        int overall_rating;
        int pace, shooting, passing, dribbling, defending, physicality;

    public:
        Player(const std::string& name, const std::string& nationality, const std::string& club, const std::string& position, int overall_rating, int pace, int shooting, int passing, int dribbling, int defending, int physicality);

    virtual ~Player() = default;

    virtual void displayInfo() const = 0;

    //Getters
    std::string getName() const { return name; }
    std::string getNationality() const { return nationality; }
    int getOverallRating() const { return overall_rating; }
};