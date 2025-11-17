#include "player.h"
#include "building.h"
#include <algorithm>
#include <iostream>

// Инициализация статических констант
const double Player::BASE_HOUSE_PRICE = 10000.0;
const double Player::BASE_HOUSE_DEMAND = 1000.0;
const double Player::BASE_MARKET_REVENUE = 20000.0;

Player::Player(QString playerName, int playerId, QColor playerColor) {
    name = playerName;
    money = INITIAL_MONEY;
    id = playerId;
    color = playerColor;
    totalRevenue = 0;
    totalConstructionCost = 0;
    totalAdvertisingCost = 0;
    isBankrupt = false;
}

Player::~Player() {
    for (Building* building : buildings) {
        delete building;
    }
}

void Player::updateState(const QList<int>& newHouseCells, const QList<int>& newMarketCells, int currentMonth) {
    Season currentSeason = getSeason(currentMonth);
    processConstruction(newHouseCells, newMarketCells);
    processMonthlyOperations(currentSeason);
    previousHouseCells = currentHouseCells;
    previousMarketCells = currentMarketCells;
    currentHouseCells = newHouseCells;
    currentMarketCells = newMarketCells;
}

void Player::processConstruction(const QList<int>& newHouseCells, const QList<int>& newMarketCells) {
    if (isBankrupt) return;

    for (int cell : newHouseCells) {
        if (!previousHouseCells.contains(cell) && !hasBuildingInCell(cell)) {
            Building* newHouse = new Building(Building::HOUSE_CONCRETE, id, HOUSE_BUILD_TIME, HOUSE_COST, color, cell);
            newHouse->setTotalArea(5000.0);
            newHouse->setPricePerSqm(BASE_HOUSE_PRICE);
            buildings.append(newHouse);
        }
    }

    for (int cell : newMarketCells) {
        if (!previousMarketCells.contains(cell) && !hasBuildingInCell(cell)) {
            Building* newMarket = new Building(Building::MARKET, id, MARKET_BUILD_TIME, MARKET_COST, color, cell);
            buildings.append(newMarket);
        }
    }
}

void Player::processMonthlyOperations(Season season) {
    if (isBankrupt) return;

    for (Building* building : buildings) {
        building->setMonthlyProfit(0.0);
    }

    // 1. Выплата месячных затрат на строительство
    for (Building* building : buildings) {
        if (!building->getIsCompleted()) {
            double monthlyCost = building->getCost() / building->getBuildTime();
            if (money >= monthlyCost) {
                money -= monthlyCost;
                building->progressMonth();
                totalConstructionCost += monthlyCost;
                building->setMonthlyProfit(-monthlyCost);
            } else {
                building->setMonthlyProfit(0);
            }
        }
    }

    // 2. Получение дохода от продажи жилья
    double housingDemand = getHousingDemand(season);
    for (Building* building : buildings) {
        if ((building->getType() == Building::HOUSE_CONCRETE ||
             building->getType() == Building::HOUSE_WOOD ||
             building->getType() == Building::HOUSE_BRICK) &&
            building->getIsCompleted()) {
            processHousingSales(building, housingDemand);
        }
    }

    // 3. Получение дохода от магазинов
    double marketRevenue = getMarketRevenue(season);
    for (Building* building : buildings) {
        if (building->getType() == Building::MARKET && building->getIsCompleted()) {
            processMarketRevenue(building, marketRevenue);
        }
    }

    // 4. Автоматическое увеличение цены в ходе строительства
    for (Building* building : buildings) {
        if (!building->getIsCompleted() &&
            (building->getType() == Building::HOUSE_CONCRETE ||
             building->getType() == Building::HOUSE_WOOD ||
             building->getType() == Building::HOUSE_BRICK)) {
            double progress = static_cast<double>(building->getMonthsBuilt()) / building->getBuildTime();
            double newPrice = BASE_HOUSE_PRICE * (1.0 + progress * 0.4);
            building->setPricePerSqm(newPrice);
        }
    }

    lastMonthProfits.clear();
    for (Building* building : buildings) {
        double profit = building->getMonthlyProfit();
        if (profit != 0) {
            lastMonthProfits.append(qMakePair(building->getCellIndex(), profit));
        }
    }

    if (money < 0) {
        isBankrupt = true;
    }
}

void Player::processHousingSales(Building* house, double baseDemand) {
    if (house->getTotalArea() > house->getSoldArea()) {
        double availableArea = house->getTotalArea() - house->getSoldArea();
        double demandModifier = 1.0;
        double effectiveDemand = baseDemand * demandModifier;
        double priceModifier = 1.0 - (house->getPricePerSqm() - BASE_HOUSE_PRICE) / BASE_HOUSE_PRICE * 0.1;

        double potentialSales = std::min(availableArea, effectiveDemand * priceModifier);
        double revenue = potentialSales * house->getPricePerSqm();

        house->setSoldArea(house->getSoldArea() + potentialSales);
        money += revenue;
        totalRevenue += revenue;
        house->setMonthlyProfit(revenue);
    }
}

void Player::processMarketRevenue(Building* market, double baseRevenue) {
    double revenueModifier = 1.0;
    double revenue = baseRevenue * revenueModifier;

    money += revenue;
    totalRevenue += revenue;
    market->setMonthlyProfit(revenue);
}

Player::Season Player::getSeason(int month) const {
    // 3 месяца на сезон, блять
    int seasonIndex = (month / 3) % 4;
    switch(seasonIndex) {
    case 0: return Season::SPRING;
    case 1: return Season::SUMMER;
    case 2: return Season::AUTUMN;
    case 3: return Season::WINTER;
    default: return Season::SPRING;
    }
}

double Player::getHousingDemand(Season season) const {
    double baseDemand = BASE_HOUSE_DEMAND;

    switch (season) {
    case Season::SPRING: return baseDemand * 1.2;
    case Season::SUMMER: return baseDemand * 1.1;
    case Season::AUTUMN: return baseDemand * 1.5;
    case Season::WINTER: return baseDemand * 0.8;
    }
    return baseDemand;
}

double Player::getMarketRevenue(Season season) const {
    double baseRevenue = BASE_MARKET_REVENUE;

    switch (season) {
    case Season::SPRING: return baseRevenue * 1.0;
    case Season::SUMMER: return baseRevenue * 1.1;
    case Season::AUTUMN: return baseRevenue * 1.3;
    case Season::WINTER: return baseRevenue * 1.6;
    }
    return baseRevenue;
}

bool Player::hasBuildingInCell(int cellIndex) const {
    for (Building* building : buildings) {
        if (building->getCellIndex() == cellIndex) {
            return true;
        }
    }
    return false;
}

double Player::calculateTotalCapital() const {
    if (isBankrupt) return 0.0;

    double capital = money;

    for (Building* building : buildings) {
        if (building->getType() == Building::MARKET) {
            if (building->getIsCompleted()) {
                capital += building->getCost() * 1.6;
            } else {
                capital += (building->getCost() * building->getMonthsBuilt()) / building->getBuildTime();
            }
        } else {
            if (building->getIsCompleted()) {
                double unsoldArea = building->getTotalArea() - building->getSoldArea();
                double costPerSqm = building->getCost() / building->getTotalArea();
                capital += unsoldArea * costPerSqm;
            } else {
                capital += (building->getCost() * building->getMonthsBuilt()) / building->getBuildTime();
            }
        }
    }

    return capital;
}

QList<Player::BuildingInfo> Player::getHouseBuildings() const {
    QList<BuildingInfo> houses;
    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE_CONCRETE ||
            building->getType() == Building::HOUSE_WOOD ||
            building->getType() == Building::HOUSE_BRICK) {
            BuildingInfo info;
            info.cellIndex = building->getCellIndex();
            info.buildProgress = building->getMonthsBuilt();
            info.totalBuildTime = building->getBuildTime();
            info.isCompleted = building->getIsCompleted();
            info.type = building->getType();
            info.monthlyProfit = building->getMonthlyProfit();
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
            info.monthlyProfit = building->getMonthlyProfit();
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
        info.monthlyProfit = building->getMonthlyProfit();
        allBuildings.append(info);
    }
    return allBuildings;
}

QList<int> Player::getHouseCells() const {
    QList<int> houseCells;
    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE_CONCRETE ||
            building->getType() == Building::HOUSE_WOOD ||
            building->getType() == Building::HOUSE_BRICK) {
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

bool Player::canBuild(Building::Type type) {
    if (isBankrupt) return false;

    int cost = 0;
    if (type == Building::HOUSE_CONCRETE || type == Building::HOUSE_WOOD || type == Building::HOUSE_BRICK) {
        cost = HOUSE_COST;
    } else if (type == Building::MARKET) {
        cost = MARKET_COST;
    }
    return money >= cost;
}

Building* Player::build(Building::Type type, int cellIndex) {
    if (!canBuild(type) || hasBuildingInCell(cellIndex)) {
        return nullptr;
    }

    int buildTime = 0;
    int cost = 0;

    if (type == Building::HOUSE_CONCRETE || type == Building::HOUSE_WOOD || type == Building::HOUSE_BRICK) {
        buildTime = HOUSE_BUILD_TIME;
        cost = HOUSE_COST;
    } else if (type == Building::MARKET) {
        buildTime = MARKET_BUILD_TIME;
        cost = MARKET_COST;
    }

    Building* newBuilding = new Building(type, id, buildTime, cost, color, cellIndex);

    if (type == Building::HOUSE_CONCRETE || type == Building::HOUSE_WOOD || type == Building::HOUSE_BRICK) {
        newBuilding->setTotalArea(5000.0);
        newBuilding->setPricePerSqm(BASE_HOUSE_PRICE);
    }

    buildings.append(newBuilding);
    money -= cost;
    totalConstructionCost += cost;

    if (type == Building::HOUSE_CONCRETE || type == Building::HOUSE_WOOD || type == Building::HOUSE_BRICK) {
        currentHouseCells.append(cellIndex);
    } else {
        currentMarketCells.append(cellIndex);
    }

    return newBuilding;
}

void Player::processMonth() {
    updateState(currentHouseCells, currentMarketCells, 0);
}

QList<QPair<int, double>> Player::getLastMonthProfits() const {
    return lastMonthProfits;
}

void Player::setLastMonthProfits(const QList<QPair<int, double>>& profits) {
    lastMonthProfits = profits;
}

void Player::clearLastMonthProfits() {
    lastMonthProfits.clear();
}
