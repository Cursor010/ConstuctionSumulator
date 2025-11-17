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
    isBankrupt = false;
}

Player::~Player() {
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл вместо range-based
        delete buildings[i];
    }
}

void Player::processMonth(const QList<Player*>& allPlayers, int currentMonth) {
    if (isBankrupt) return;

    // Очищаем прибыль за прошлый месяц
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        buildings[i]->setMonthlyProfit(0.0);
    }

    // 1. Выплата месячных затрат на строительство
    payConstructionCosts();

    // 2. Получение дохода от магазинов (даже если нет соседних домов - базовая прибыль)
    receiveMarketRevenue(allPlayers, currentMonth);

    // 3. Обновление цен на жилье в ходе строительства
    updateHousingPrices();

    // 4. Получение дохода от продажи жилья (обрабатывается в риэлторском агентстве)
    // Этот доход будет добавлен позже в RealEstateAgency

    // Запоминаем прибыли для отображения
    lastMonthProfits.clear();
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        double profit = buildings[i]->getMonthlyProfit();
        if (profit != 0) {
            lastMonthProfits.append(qMakePair(buildings[i]->getCellIndex(), profit));
        }
    }

    // Проверка на банкротство
    if (money < 0) {
        isBankrupt = true;
    }
}

void Player::payConstructionCosts() {
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        Building* building = buildings[i];
        if (!building->getIsCompleted()) {
            double monthlyCost = static_cast<double>(building->getCost()) / building->getBuildTime();
            if (money >= monthlyCost) {
                money -= monthlyCost;
                building->progressMonth();
                building->addToMonthlyProfit(-monthlyCost);

                // Показываем прогресс строительства
                if (building->getMonthsBuilt() < building->getBuildTime()) {
                    std::cout << "Player " << id << " paid construction cost for cell "
                              << building->getCellIndex() << ": -" << monthlyCost << std::endl;
                } else {
                    std::cout << "Player " << id << " completed building in cell "
                              << building->getCellIndex() << std::endl;
                }
            } else {
                // Не можем платить - здание не прогрессирует
                building->addToMonthlyProfit(0);
                std::cout << "Player " << id << " cannot pay construction cost for cell "
                          << building->getCellIndex() << std::endl;
            }
        }
    }
}

void Player::receiveMarketRevenue(const QList<Player*>& allPlayers, int currentMonth) {
    Season currentSeason = getSeason(currentMonth);
    double baseRevenue = getMarketRevenue(currentSeason);

    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        Building* building = buildings[i];
        if (building->getType() == Building::MARKET && building->getIsCompleted()) {
            // Базовая прибыль магазина
            double revenue = baseRevenue;

            // Бонус за соседние дома (если есть)
            int neighborHouses = countNeighborHouses(building->getCellIndex(), allPlayers);
            if (neighborHouses > 0) {
                double neighborBonus = 1.0 + (neighborHouses * GameConfig::MARKET_NEIGHBOR_BONUS);
                revenue *= neighborBonus;
            }

            money += revenue;
            building->addToMonthlyProfit(revenue);

            std::cout << "Player " << id << " market revenue in cell "
                      << building->getCellIndex() << ": +" << revenue
                      << " (neighbors: " << neighborHouses << ")" << std::endl;
        }
    }
}

void Player::updateHousingPrices() {
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        Building* building = buildings[i];
        if ((building->getType() == Building::HOUSE_CONCRETE ||
             building->getType() == Building::HOUSE_WOOD ||
             building->getType() == Building::HOUSE_BRICK) &&
            !building->getIsCompleted()) {

            // Цена растет в ходе строительства
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

            double newPrice = basePrice * (1.0 + progress * 0.3);
            building->setPricePerSqm(newPrice);
        }
    }
}

int Player::countNeighborHouses(int cellIndex, const QList<Player*>& allPlayers) const {
    int count = 0;
    QList<int> neighbors = getNeighborCells(cellIndex);

    for (int i = 0; i < neighbors.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        int neighborCell = neighbors[i];
        for (int j = 0; j < allPlayers.size(); ++j) { // ИСПРАВЛЕНО: используем индексный цикл
            Player* player = allPlayers[j];
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

    for (int i = 0; i < neighbors.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        int neighborCell = neighbors[i];
        for (int j = 0; j < allPlayers.size(); ++j) { // ИСПРАВЛЕНО: используем индексный цикл
            Player* player = allPlayers[j];
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

    // Соседние клетки (включая диагональные)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

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
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        if (buildings[i]->getCellIndex() == cellIndex) {
            return true;
        }
    }
    return false;
}

double Player::calculateTotalCapital() const {
    if (isBankrupt) return 0.0;

    double capital = money;

    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        Building* building = buildings[i];
        if (building->getType() == Building::MARKET) {
            // Стоимость построенных супермаркетов на 60% выше потраченной суммы
            if (building->getIsCompleted()) {
                capital += building->getCost() * 1.6;
            } else {
                // Стоимость строящихся - потраченная сумма
                capital += (building->getCost() * building->getMonthsBuilt()) / building->getBuildTime();
            }
        } else {
            // Для домов - стоимость непроданного жилья по себестоимости
            if (building->getIsCompleted()) {
                double unsoldArea = building->getTotalArea() - building->getSoldArea();
                double costPerSqm = building->getCost() / building->getTotalArea();
                capital += unsoldArea * costPerSqm;
            } else {
                // Стоимость строящихся домов
                capital += (building->getCost() * building->getMonthsBuilt()) / building->getBuildTime();
            }
        }
    }

    return capital;
}

QList<Player::BuildingInfo> Player::getAllBuildings() const {
    QList<BuildingInfo> allBuildings;
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        Building* building = buildings[i];
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
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        Building* building = buildings[i];
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
    for (int i = 0; i < buildings.size(); ++i) { // ИСПРАВЛЕНО: используем индексный цикл
        Building* building = buildings[i];
        if (building->getType() == Building::MARKET) {
            marketCells.append(building->getCellIndex());
        }
    }
    return marketCells;
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

    return newBuilding;
}

QList<QPair<int, double>> Player::getLastMonthProfits() const {
    return lastMonthProfits;
}

void Player::clearLastMonthProfits() {
    lastMonthProfits.clear();
}
