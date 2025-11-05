#pragma once
#include "Building.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

class Player {
public:
    std::string name;
    sf::Color color;
    double money;
    std::vector<std::unique_ptr<Building>> buildings;
    bool isHuman;
    bool builtHouseThisTurn;
    bool builtSupermarketThisTurn;

    Player(std::string n, sf::Color c, double startMoney, bool human = true);

    bool canBuildHouse() const;
    bool canBuildSupermarket() const;
    void buildHouse(sf::Vector2f position);
    void buildSupermarket(sf::Vector2f position);
    void updateMonthly();
    void draw(sf::RenderWindow& window);
    void resetTurn();
};