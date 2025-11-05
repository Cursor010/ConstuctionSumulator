#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Building {
public:
    sf::Vector2f position;
    sf::Color color;
    std::string type;
    double cost;
    int constructionTime;
    int monthsBuilt;
    bool isCompleted;
    std::string ownerName;

    Building(sf::Vector2f pos, sf::Color col, std::string t, double c, int time, std::string owner);
    virtual ~Building() = default;

    virtual void update();
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual std::string getInfo() = 0;
    virtual sf::FloatRect getBounds() = 0;
};

class House : public Building {
private:
    sf::RectangleShape shape;
    double squareMeters;

public:
    House(sf::Vector2f pos, sf::Color col, std::string owner);
    void draw(sf::RenderWindow& window) override;
    std::string getInfo() override;
    sf::FloatRect getBounds() override;
};

class Supermarket : public Building {
private:
    sf::CircleShape shape;

public:
    Supermarket(sf::Vector2f pos, sf::Color col, std::string owner);
    void draw(sf::RenderWindow& window) override;
    std::string getInfo() override;
    sf::FloatRect getBounds() override;
};