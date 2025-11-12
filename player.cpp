#include "player.h"
#include "building.h"
#include <algorithm>

const int HOUSE_COST = 8000000;
const int MARKET_COST = 2500000;
const int HOUSE_BUILD_TIME = 6;
const int MARKET_BUILD_TIME = 3;
const int INITIAL_MONEY = 37000000;

Player::Player(QString playerName, int playerId, QColor playerColor) {
    name = playerName;
    money = INITIAL_MONEY;
    id = playerId;
    color = playerColor;
    advertisingHousing = 0;
    advertisingGoods = 0;
}

Player::~Player() {
    for (Building* building : buildings) {
        delete building;
    }
}

bool Player::canBuild(Building::Type type) {
    int cost = (type == Building::HOUSE) ? HOUSE_COST : MARKET_COST;
    return money >= cost;
}

Building* Player::build(Building::Type type, int cellIndex) {
    int cost = (type == Building::HOUSE) ? HOUSE_COST : MARKET_COST;
    int buildTime = (type == Building::HOUSE) ? HOUSE_BUILD_TIME : MARKET_BUILD_TIME;

    if (money >= cost) {
        money -= cost;
        Building* newBuilding = new Building(type, id, buildTime, cost, color);
        buildings.append(newBuilding);
        return newBuilding;
    }
    return nullptr;
}

void Player::processMonth() {
    // Обновляем прогресс строительства для всех зданий
    for (Building* building : buildings) {
        building->progressMonth();
    }

    // Затраты на строительство (только для незавершенных)
    for (Building* building : buildings) {
        if (!building->getIsCompleted()) {
            money -= building->getConstructionCostPerMonth();
        }
    }

    // Доход от продажи жилья
    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE && building->getIsCompleted()) {
            int apartmentsToSell = 10 + advertisingHousing / 1000 * 5;
            int pricePerApartment = 100000 + building->getMonthsBuilt() * 5000;

            int actualSales = std::min(apartmentsToSell, building->getTotalApartments() - building->getSoldApartments());
            building->setSoldApartments(building->getSoldApartments() + actualSales);
            money += actualSales * pricePerApartment;
        }
    }

    // Доход от магазинов
    for (Building* building : buildings) {
        if (building->getType() == Building::MARKET && building->getIsCompleted()) {
            int baseProfit = 200000;
            int advertisingBonus = advertisingGoods / 500 * 3;
            int totalProfit = baseProfit * (100 + advertisingBonus) / 100;
            money += totalProfit;
        }
    }
}

int Player::calculateTotalCapital() {
    int capital = money;

    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE) {
            if (building->getIsCompleted()) {
                int unsoldApartments = building->getTotalApartments() - building->getSoldApartments();
                int costPerApartment = building->getCost() / building->getTotalApartments();
                capital += unsoldApartments * costPerApartment;
            } else {
                capital += building->getMonthsBuilt() * building->getConstructionCostPerMonth();
            }
        } else if (building->getType() == Building::MARKET && building->getIsCompleted()) {
            capital += building->getCost() * 1.6;
        }
    }

    return capital;
}
