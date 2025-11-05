#include "Player.h"

Player::Player(std::string n, sf::Color c, double startMoney, bool human)
    : name(n), color(c), money(startMoney), isHuman(human),
    builtHouseThisTurn(false), builtSupermarketThisTurn(false) {}

bool Player::canBuildHouse() const {
    return money >= 8.0 && !builtHouseThisTurn;
}

bool Player::canBuildSupermarket() const {
    return money >= 2.5 && !builtSupermarketThisTurn;
}

void Player::buildHouse(sf::Vector2f position) {
    if (canBuildHouse()) {
        money -= 8.0;
        buildings.push_back(std::make_unique<House>(position, color, name));
        builtHouseThisTurn = true;
    }
}

void Player::buildSupermarket(sf::Vector2f position) {
    if (canBuildSupermarket()) {
        money -= 2.5;
        buildings.push_back(std::make_unique<Supermarket>(position, color, name));
        builtSupermarketThisTurn = true;
    }
}

void Player::updateMonthly() {
    for (auto& building : buildings) {
        if (building->isCompleted) {
            if (building->type == "Supermarket") {
                money += 0.5;
            }
            else if (building->type == "House") {
                money += 1.0;
            }
        }
        building->update();
    }
}

void Player::draw(sf::RenderWindow& window) {
    for (auto& building : buildings) {
        building->draw(window);
    }
}

void Player::resetTurn() {
    builtHouseThisTurn = false;
    builtSupermarketThisTurn = false;
}