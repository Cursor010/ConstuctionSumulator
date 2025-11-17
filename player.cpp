#include "player.h"
#include "building.h"
#include "gameconfig.h"
#include <algorithm>
#include <iostream>
#include <cmath>

Player::Player(QString playerName, int playerId, QColor playerColor) {
    name = playerName;
    money = GameConfig::INITIAL_MONEY;
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

void Player::updateState(const QList<int>& newHouseCells, const QList<int>& newMarketCells, int currentMonth, const QList<Player*>& allPlayers) {
    Season currentSeason = getSeason(currentMonth);
    processConstruction(newHouseCells, newMarketCells);
    processMonthlyOperations(currentSeason, allPlayers);
    previousHouseCells = currentHouseCells;
    previousMarketCells = currentMarketCells;
    currentHouseCells = newHouseCells;
    currentMarketCells = newMarketCells;
}

void Player::processConstruction(const QList<int>& newHouseCells, const QList<int>& newMarketCells) {
    if (isBankrupt) return;

    // Эта функция теперь не используется для строительства, строительство через метод build()
}

void Player::processMonthlyOperations(Season season, const QList<Player*>& allPlayers) {
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
            processHousingSales(building, housingDemand, allPlayers);
        }
    }

    // 3. Получение дохода от магазинов
    double marketRevenue = getMarketRevenue(season);
    for (Building* building : buildings) {
        if (building->getType() == Building::MARKET && building->getIsCompleted()) {
            processMarketRevenue(building, marketRevenue, allPlayers);
        }
    }

    // 4. Автоматическое увеличение цены в ходе строительства
    for (Building* building : buildings) {
        if (!building->getIsCompleted() &&
            (building->getType() == Building::HOUSE_CONCRETE ||
             building->getType() == Building::HOUSE_WOOD ||
             building->getType() == Building::HOUSE_BRICK)) {
            double progress = static_cast<double>(building->getMonthsBuilt()) / building->getBuildTime();
            double basePrice = 0;

            switch(building->getType()) {
            case Building::HOUSE_CONCRETE:
                basePrice = GameConfig::CONCRETE_HOUSE_BASE_PRICE;
                break;
            case Building::HOUSE_WOOD:
                basePrice = GameConfig::WOOD_HOUSE_BASE_PRICE;
                break;
            case Building::HOUSE_BRICK:
                basePrice = GameConfig::BRICK_HOUSE_BASE_PRICE;
                break;
            default:
                basePrice = GameConfig::BRICK_HOUSE_BASE_PRICE;
            }

            double newPrice = basePrice * (1.0 + progress * 0.4);
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

void Player::processHousingSales(Building* house, double baseDemand, const QList<Player*>& allPlayers) {
    if (house->getTotalArea() > house->getSoldArea()) {
        double availableArea = house->getTotalArea() - house->getSoldArea();

        // Учитываем соседние магазины для увеличения спроса
        int neighborMarkets = countNeighborMarkets(house->getCellIndex(), allPlayers);
        double neighborBonus = 1.0 + (neighborMarkets * GameConfig::HOUSE_NEIGHBOR_BONUS);

        double demandModifier = neighborBonus;
        double effectiveDemand = baseDemand * demandModifier;

        // Модификатор цены (чем дороже, тем меньше спрос)
        double basePrice = 0;
        switch(house->getType()) {
        case Building::HOUSE_CONCRETE:
            basePrice = GameConfig::CONCRETE_HOUSE_BASE_PRICE;
            break;
        case Building::HOUSE_WOOD:
            basePrice = GameConfig::WOOD_HOUSE_BASE_PRICE;
            break;
        case Building::HOUSE_BRICK:
            basePrice = GameConfig::BRICK_HOUSE_BASE_PRICE;
            break;
        default:
            basePrice = GameConfig::BRICK_HOUSE_BASE_PRICE;
        }

        double priceModifier = 1.0 - (house->getPricePerSqm() - basePrice) / basePrice * 0.1;

        double potentialSales = std::min(availableArea, effectiveDemand * priceModifier);
        double revenue = potentialSales * house->getPricePerSqm();

        house->setSoldArea(house->getSoldArea() + potentialSales);
        money += revenue;
        totalRevenue += revenue;
        house->setMonthlyProfit(house->getMonthlyProfit() + revenue);
    }
}

void Player::processMarketRevenue(Building* market, double baseRevenue, const QList<Player*>& allPlayers) {
    // Учитываем соседние дома для увеличения дохода
    int neighborHouses = countNeighborHouses(market->getCellIndex(), allPlayers);
    double neighborBonus = 1.0 + (neighborHouses * GameConfig::MARKET_NEIGHBOR_BONUS);

    double revenueModifier = neighborBonus;
    double revenue = baseRevenue * revenueModifier;

    money += revenue;
    totalRevenue += revenue;
    market->setMonthlyProfit(market->getMonthlyProfit() + revenue);
}

int Player::countNeighborHouses(int cellIndex, const QList<Player*>& allPlayers) const {
    int count = 0;
    QList<int> neighbors = getNeighborCells(cellIndex);

    for (int neighborCell : neighbors) {
        for (Player* player : allPlayers) {
            QList<int> houseCells = player->getHouseCells();
            if (houseCells.contains(neighborCell)) {
                count++;
                break;
            }
        }
    }

    return count;
}

int Player::countNeighborMarkets(int cellIndex, const QList<Player*>& allPlayers) const {
    int count = 0;
    QList<int> neighbors = getNeighborCells(cellIndex);

    for (int neighborCell : neighbors) {
        for (Player* player : allPlayers) {
            QList<int> marketCells = player->getMarketCells();
            if (marketCells.contains(neighborCell)) {
                count++;
                break;
            }
        }
    }

    return count;
}

QList<int> Player::getNeighborCells(int cellIndex) const {
    QList<int> neighbors;
    int row = cellIndex / 5;
    int col = cellIndex % 5;

    // Проверяем все 8 направлений
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // пропускаем саму клетку

            int newRow = row + i;
            int newCol = col + j;

            if (newRow >= 0 && newRow < 5 && newCol >= 0 && newCol < 5) {
                neighbors.append(newRow * 5 + newCol);
            }
        }
    }

    return neighbors;
}

Player::Season Player::getSeason(int month) const {
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
    double baseDemand = GameConfig::BASE_HOUSE_DEMAND;

    switch (season) {
    case Season::SPRING: return baseDemand * GameConfig::SPRING_HOUSE_MODIFIER;
    case Season::SUMMER: return baseDemand * GameConfig::SUMMER_HOUSE_MODIFIER;
    case Season::AUTUMN: return baseDemand * GameConfig::AUTUMN_HOUSE_MODIFIER;
    case Season::WINTER: return baseDemand * GameConfig::WINTER_HOUSE_MODIFIER;
    }
    return baseDemand;
}

double Player::getMarketRevenue(Season season) const {
    double baseRevenue = GameConfig::BASE_MARKET_REVENUE;

    switch (season) {
    case Season::SPRING: return baseRevenue * GameConfig::SPRING_MARKET_MODIFIER;
    case Season::SUMMER: return baseRevenue * GameConfig::SUMMER_MARKET_MODIFIER;
    case Season::AUTUMN: return baseRevenue * GameConfig::AUTUMN_MARKET_MODIFIER;
    case Season::WINTER: return baseRevenue * GameConfig::WINTER_MARKET_MODIFIER;
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
    if (type == Building::HOUSE_CONCRETE) {
        cost = GameConfig::CONCRETE_HOUSE_COST;
    } else if (type == Building::HOUSE_WOOD) {
        cost = GameConfig::WOOD_HOUSE_COST;
    } else if (type == Building::HOUSE_BRICK) {
        cost = GameConfig::BRICK_HOUSE_COST;
    } else if (type == Building::MARKET) {
        cost = GameConfig::MARKET_COST;
    }
    return money >= cost;
}

Building* Player::build(Building::Type type, int cellIndex) {
    if (!canBuild(type) || hasBuildingInCell(cellIndex)) {
        return nullptr;
    }

    int buildTime = 0;
    int cost = 0;
    double area = 0;
    double basePrice = 0;

    if (type == Building::HOUSE_CONCRETE) {
        buildTime = GameConfig::CONCRETE_HOUSE_BUILD_TIME;
        cost = GameConfig::CONCRETE_HOUSE_COST;
        area = GameConfig::CONCRETE_HOUSE_AREA;
        basePrice = GameConfig::CONCRETE_HOUSE_BASE_PRICE;
    } else if (type == Building::HOUSE_WOOD) {
        buildTime = GameConfig::WOOD_HOUSE_BUILD_TIME;
        cost = GameConfig::WOOD_HOUSE_COST;
        area = GameConfig::WOOD_HOUSE_AREA;
        basePrice = GameConfig::WOOD_HOUSE_BASE_PRICE;
    } else if (type == Building::HOUSE_BRICK) {
        buildTime = GameConfig::BRICK_HOUSE_BUILD_TIME;
        cost = GameConfig::BRICK_HOUSE_COST;
        area = GameConfig::BRICK_HOUSE_AREA;
        basePrice = GameConfig::BRICK_HOUSE_BASE_PRICE;
    } else if (type == Building::MARKET) {
        buildTime = GameConfig::MARKET_BUILD_TIME;
        cost = GameConfig::MARKET_COST;
    }

    Building* newBuilding = new Building(type, id, buildTime, cost, color, cellIndex);

    if (type == Building::HOUSE_CONCRETE || type == Building::HOUSE_WOOD || type == Building::HOUSE_BRICK) {
        newBuilding->setTotalArea(area);
        newBuilding->setPricePerSqm(basePrice);
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

void Player::processMonth(const QList<Player*>& allPlayers) {
    updateState(currentHouseCells, currentMarketCells, 0, allPlayers);
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
