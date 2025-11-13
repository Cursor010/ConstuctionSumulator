#include "player.h"
#include "building.h"
#include <algorithm>
#include <iostream>

// Инициализация статических констант
const double Player::BASE_HOUSE_PRICE = 10000.0;

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
    // Определяем сезон для расчета модификаторов
    Season currentSeason = getSeason(currentMonth);

    // Обрабатываем строительство новых зданий
    processConstruction(newHouseCells, newMarketCells);

    // Обрабатываем ежемесячные операции
    processMonthlyOperations(currentSeason);

    // Сохраняем текущее состояние для следующего месяца
    previousHouseCells = currentHouseCells;
    previousMarketCells = currentMarketCells;
    currentHouseCells = newHouseCells;
    currentMarketCells = newMarketCells;
}

void Player::processConstruction(const QList<int>& newHouseCells, const QList<int>& newMarketCells) {
    if (isBankrupt) return;

    // Находим новые дома (которые есть в новых данных, но не было в предыдущих)
    for (int cell : newHouseCells) {
        if (!previousHouseCells.contains(cell) && !hasBuildingInCell(cell)) {
            // Создаем новый дом
            Building* newHouse = new Building(Building::HOUSE, id, HOUSE_BUILD_TIME, HOUSE_COST, color, cell);
            newHouse->setTotalArea(5000.0); // примерная площадь
            newHouse->setPricePerSqm(BASE_HOUSE_PRICE);
            buildings.append(newHouse);
            std::cout << "Player " << id << " started building house in cell " << cell << std::endl;
        }
    }

    // Находим новые магазины
    for (int cell : newMarketCells) {
        if (!previousMarketCells.contains(cell) && !hasBuildingInCell(cell)) {
            // Создаем новый магазин
            Building* newMarket = new Building(Building::MARKET, id, MARKET_BUILD_TIME, MARKET_COST, color, cell);
            buildings.append(newMarket);
            std::cout << "Player " << id << " started building market in cell " << cell << std::endl;
        }
    }
}

void Player::processMonthlyOperations(Season season) {
    if (isBankrupt) return;

    // Сбрасываем прибыль для всех зданий
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
                // Устанавливаем отрицательную прибыль для строящихся зданий
                building->setMonthlyProfit(-monthlyCost);

                if (building->getIsCompleted()) {
                    std::cout << "Player " << id << " completed building in cell " << building->getCellIndex() << std::endl;
                }
            } else {
                // Не хватает денег - строительство замедляется
                building->setMonthlyProfit(0); // Прибыль 0, так как строительство не прогрессирует
                std::cout << "Player " << id << " cannot pay construction cost for cell " << building->getCellIndex() << std::endl;
            }
        }
    }

    // 2. Получение дохода от продажи жилья
    double housingDemand = getHousingDemand(season);
    for (Building* building : buildings) {
        if (building->getType() == Building::HOUSE && building->getIsCompleted()) {
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
        if (!building->getIsCompleted() && building->getType() == Building::HOUSE) {
            double progress = static_cast<double>(building->getMonthsBuilt()) / building->getBuildTime();
            double newPrice = BASE_HOUSE_PRICE * (1.0 + progress * 0.4); // Цена растет на 40% к завершению
            building->setPricePerSqm(newPrice);
        }
    }

    // Сохраняем прибыль за последний месяц
    lastMonthProfits.clear();
    for (Building* building : buildings) {
        double profit = building->getMonthlyProfit();
        if (profit != 0) {
            lastMonthProfits.append(qMakePair(building->getCellIndex(), profit));
        }
    }

    // Проверка на банкротство
    if (money < 0) {
        isBankrupt = true;
        std::cout << "Player " << id << " is BANKRUPT!" << std::endl;
    }
}

void Player::processHousingSales(Building* house, double baseDemand) {
    if (house->getTotalArea() > house->getSoldArea()) {
        double availableArea = house->getTotalArea() - house->getSoldArea();

        // Модификаторы спроса (пока без рекламы и конкуренции)
        double demandModifier = 1.0;
        double effectiveDemand = baseDemand * demandModifier;

        // Модификатор цены (чем выше цена, тем меньше спрос)
        double priceModifier = 1.0 - (house->getPricePerSqm() - BASE_HOUSE_PRICE) / BASE_HOUSE_PRICE * 0.1;

        double potentialSales = std::min(availableArea, effectiveDemand * priceModifier);
        double revenue = potentialSales * house->getPricePerSqm();

        house->setSoldArea(house->getSoldArea() + potentialSales);
        money += revenue;
        totalRevenue += revenue;

        // Устанавливаем положительную прибыль для построенного дома
        house->setMonthlyProfit(revenue);

        if (potentialSales > 0) {
            std::cout << "Player " << id << " sold " << potentialSales << " sqm for " << revenue << std::endl;
        }
    }
}

void Player::processMarketRevenue(Building* market, double baseRevenue) {
    // Модификаторы дохода (пока без рекламы и влияния домов)
    double revenueModifier = 1.0;
    double revenue = baseRevenue * revenueModifier;

    money += revenue;
    totalRevenue += revenue;

    // Устанавливаем положительную прибыль для магазина
    market->setMonthlyProfit(revenue);

    std::cout << "Player " << id << " market revenue: " << revenue << std::endl;
}

Season Player::getSeason(int month) const {
    int monthInYear = month % 12;
    if (monthInYear >= 2 && monthInYear <= 4) return Season::SPRING;
    if (monthInYear >= 5 && monthInYear <= 7) return Season::SUMMER;
    if (monthInYear >= 8 && monthInYear <= 10) return Season::AUTUMN;
    return Season::WINTER;
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
                capital += building->getCost() * 1.6; // На 60% выше стоимости строительства
            } else {
                // Стоимость незавершенного строительства
                capital += (building->getCost() * building->getMonthsBuilt()) / building->getBuildTime();
            }
        } else { // HOUSE
            if (building->getIsCompleted()) {
                double unsoldArea = building->getTotalArea() - building->getSoldArea();
                double costPerSqm = building->getCost() / building->getTotalArea();
                capital += unsoldArea * costPerSqm; // Непроданное жилье по себестоимости
            } else {
                capital += (building->getCost() * building->getMonthsBuilt()) / building->getBuildTime();
            }
        }
    }

    return capital;
}

// Геттеры для получения информации о зданиям
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

bool Player::canBuild(Building::Type type) {
    if (isBankrupt) return false;

    int cost = (type == Building::HOUSE) ? HOUSE_COST : MARKET_COST;
    return money >= cost;
}

Building* Player::build(Building::Type type, int cellIndex) {
    if (!canBuild(type) || hasBuildingInCell(cellIndex)) {
        return nullptr;
    }

    int buildTime = (type == Building::HOUSE) ? HOUSE_BUILD_TIME : MARKET_BUILD_TIME;
    int cost = (type == Building::HOUSE) ? HOUSE_COST : MARKET_COST;

    Building* newBuilding = new Building(type, id, buildTime, cost, color, cellIndex);

    if (type == Building::HOUSE) {
        newBuilding->setTotalArea(5000.0);
        newBuilding->setPricePerSqm(BASE_HOUSE_PRICE);
    }

    buildings.append(newBuilding);
    money -= cost;
    totalConstructionCost += cost;

    // Обновляем списки клеток
    if (type == Building::HOUSE) {
        currentHouseCells.append(cellIndex);
    } else {
        currentMarketCells.append(cellIndex);
    }

    return newBuilding;
}

void Player::processMonth() {
    // Для обратной совместимости - используем текущие данные
    // В реальном коде вам нужно определить, какой месяц передавать
    updateState(currentHouseCells, currentMarketCells, 0);
}

QList<QPair<int, double>> Player::getBuildingsMonthlyProfit() const {
    QList<QPair<int, double>> profits;
    for (Building* building : buildings) {
        profits.append(qMakePair(building->getCellIndex(), building->getMonthlyProfit()));
    }
    return profits;
}

void Player::setLastMonthProfits(const QList<QPair<int, double>>& profits) {
    lastMonthProfits = profits;
}

QList<QPair<int, double>> Player::getLastMonthProfits() const {
    return lastMonthProfits;
}

void Player::clearLastMonthProfits() {
    lastMonthProfits.clear();
}
