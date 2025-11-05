#include "Building.h"

Building::Building(sf::Vector2f pos, sf::Color col, std::string t, double c, int time, std::string owner)
    : position(pos), color(col), type(t), cost(c), constructionTime(time),
    monthsBuilt(0), isCompleted(false), ownerName(owner) {}

void Building::update() {
    if (!isCompleted) {
        monthsBuilt++;
        if (monthsBuilt >= constructionTime) {
            isCompleted = true;
        }
    }
}

House::House(sf::Vector2f pos, sf::Color col, std::string owner)
    : Building(pos, col, "House", 8.0, 7, owner), squareMeters(100) {
    shape.setSize(sf::Vector2f(40, 40));
    shape.setPosition(position);
    shape.setFillColor(color);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
}

void House::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    shape.setFillColor(color);
    window.draw(shape);
}

std::string House::getInfo() {
    return "House (" + ownerName + ") - " +
        (isCompleted ? "Built" : "Building: " + std::to_string(monthsBuilt) + "/" + std::to_string(constructionTime));
}

sf::FloatRect House::getBounds() {
    return sf::FloatRect(position.x, position.y, 40, 40);
}

Supermarket::Supermarket(sf::Vector2f pos, sf::Color col, std::string owner)
    : Building(pos, col, "Supermarket", 2.5, 5, owner) {
    shape.setRadius(20);
    shape.setPosition(position);
    shape.setFillColor(color);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
}

void Supermarket::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    shape.setFillColor(color);
    window.draw(shape);
}

std::string Supermarket::getInfo() {
    return "Supermarket (" + ownerName + ") - " +
        (isCompleted ? "Operating" : "Building: " + std::to_string(monthsBuilt) + "/" + std::to_string(constructionTime));
}

sf::FloatRect Supermarket::getBounds() {
    return sf::FloatRect(position.x, position.y, 40, 40);
}