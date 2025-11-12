#include "player.h"
#include "building.h"
#include <algorithm>

<<<<<<< HEAD
const int HOUSE_COST = 8000000;
const int MARKET_COST = 2500000;
const int HOUSE_BUILD_TIME = 6;
const int MARKET_BUILD_TIME = 3;
const int INITIAL_MONEY = 37000000;

=======
>>>>>>> front
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

<<<<<<< HEAD
=======
bool Player::changeMoney(int amount) {
    if (money + amount < 0) {
        return false;
    }
    money += amount;
    return true;
}

QList<Player::BuildingInfo> Player::getHouseBuildings() const {
    QList<BuildingInfo> houses;
    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE) {
            BuildingInfo info;
            info.cellIndex = building->getCellIndex();
            info.buildProgress = building->getMonthsBuilt();
            info.totalBuildTime = building->getBuildTime();
            info.isCompleted = building->getIsCompleted();
            info.type = building->getType();
            houses.append(info);
        }
    }
    return houses;
}

QList<Player::BuildingInfo> Player::getMarketBuildings() const {
    QList<BuildingInfo> markets;
    for (Building* building : buildings) {
        if (building->getType() == Building::MARKET) {
            BuildingInfo info;
            info.cellIndex = building->getCellIndex();
            info.buildProgress = building->getMonthsBuilt();
            info.totalBuildTime = building->getBuildTime();
            info.isCompleted = building->getIsCompleted();
            info.type = building->getType();
            markets.append(info);
        }
    }
    return markets;
}

QList<Player::BuildingInfo> Player::getAllBuildings() const {
    QList<BuildingInfo> allBuildings;
    for (Building* building : buildings) {
        BuildingInfo info;
        info.cellIndex = building->getCellIndex();
        info.buildProgress = building->getMonthsBuilt();
        info.totalBuildTime = building->getBuildTime();
        info.isCompleted = building->getIsCompleted();
        info.type = building->getType();
        allBuildings.append(info);
    }
    return allBuildings;
}

QList<int> Player::getHouseCells() const {
    QList<int> houseCells;
    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE) {
            houseCells.append(building->getCellIndex());
        }
    }
    return houseCells;
}

QList<int> Player::getMarketCells() const {
    QList<int> marketCells;
    for (Building* building : buildings) {
        if (building->getType() == Building::MARKET) {
            marketCells.append(building->getCellIndex());
        }
    }
    return marketCells;
}

QList<int> Player::getAllBuildingsCells() const {
    QList<int> allCells;
    for (Building* building : buildings) {
        allCells.append(building->getCellIndex());
    }
    return allCells;
}

>>>>>>> front
bool Player::canBuild(Building::Type type) {
    int cost = (type == Building::HOUSE) ? HOUSE_COST : MARKET_COST;
    return money >= cost;
}

Building* Player::build(Building::Type type, int cellIndex) {
    int cost = (type == Building::HOUSE) ? HOUSE_COST : MARKET_COST;
    int buildTime = (type == Building::HOUSE) ? HOUSE_BUILD_TIME : MARKET_BUILD_TIME;

    if (money >= cost) {
        money -= cost;
<<<<<<< HEAD
        Building* newBuilding = new Building(type, id, buildTime, cost, color);
=======
        Building* newBuilding = new Building(type, id, buildTime, cost, color, cellIndex);
>>>>>>> front
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

<<<<<<< HEAD
    // Доход от продажи жилья
=======
    // Доход от продажи жилья (только для завершенных домов)
>>>>>>> front
    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE && building->getIsCompleted()) {
            int apartmentsToSell = 10 + advertisingHousing / 1000 * 5;
            int pricePerApartment = 100000 + building->getMonthsBuilt() * 5000;

            int actualSales = std::min(apartmentsToSell, building->getTotalApartments() - building->getSoldApartments());
            building->setSoldApartments(building->getSoldApartments() + actualSales);
            money += actualSales * pricePerApartment;
        }
    }

<<<<<<< HEAD
    // Доход от магазинов
=======
    // Доход от магазинов (только для завершенных магазинов)
>>>>>>> front
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
