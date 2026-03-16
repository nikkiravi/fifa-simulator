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
    std::string getClub() const { return club; }
    std::string getPosition() const { return position; }
    int getOverallRating() const { return overall_rating; }
    int getPace() const { return pace; }
    int getShooting() const { return shooting; }
    int getPassing() const { return passing; }
    int getDribbling() const { return dribbling; }
    int getDefending() const { return defending; }
    int getPhysicality() const { return physicality; }
};