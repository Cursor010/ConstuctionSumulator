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
    for (int i = 0; i < buildings.size(); ++i) {
        delete buildings[i];
    }
}

void Player::processMonth(const QList<Player*>& allPlayers, int currentMonth) {
    if (isBankrupt) return;

    // Очищаем прибыль за прошлый месяц
    for (int i = 0; i < buildings.size(); ++i) {
        buildings[i]->setMonthlyProfit(0.0);
    }

    // 1. Выплата месячных затрат на строительство
    payConstructionCosts();

    // 2. Продажа квартир в построенных домах
    sellHousing(allPlayers, currentMonth);

    // 3. Получение дохода от магазинов
    receiveMarketRevenue(allPlayers, currentMonth);

    // 4. Обновление цен на жилье
    updateHousingPrices();

    // Запоминаем прибыли для отображения
    lastMonthProfits.clear();
    for (int i = 0; i < buildings.size(); ++i) {
        double profit = buildings[i]->getMonthlyProfit();
        if (profit != 0) {
            lastMonthProfits.append(qMakePair(buildings[i]->getCellIndex(), profit));
        }
    }

    // Проверка на банкротство
    if (money < 0) {
        isBankrupt = true;
        std::cout << "Player " << id << " is BANKRUPT!" << std::endl;
    }
}

void Player::payConstructionCosts() {
    for (int i = 0; i < buildings.size(); ++i) {
        Building* building = buildings[i];
        if (!building->getIsCompleted()) {
            double monthlyCost = static_cast<double>(building->getCost()) / building->getBuildTime();
            if (money >= monthlyCost) {
                money -= monthlyCost;
                building->progressMonth();
                building->addToMonthlyProfit(-monthlyCost);

                std::cout << "Player " << id << " paid construction cost for cell "
                          << building->getCellIndex() << ": -" << monthlyCost << std::endl;

                // Если дом только что достроился
                if (building->getIsCompleted()) {
                    std::cout << "Player " << id << " COMPLETED building in cell "
                              << building->getCellIndex() << "! Ready to sell apartments." << std::endl;
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

void Player::sellHousing(const QList<Player*>& allPlayers, int currentMonth) {
    for (int i = 0; i < buildings.size(); ++i) {
        Building* building = buildings[i];
        // Продаем квартиры только в построенных домах, где есть непроданная площадь
        if ((building->getType() == Building::HOUSE_CONCRETE ||
             building->getType() == Building::HOUSE_WOOD ||
             building->getType() == Building::HOUSE_BRICK) &&
            building->getIsCompleted() &&
            building->getSoldArea() < building->getTotalArea()) {

            double revenue = calculateHouseSales(building, allPlayers, currentMonth);
            if (revenue > 0) {
                money += revenue;
                building->addToMonthlyProfit(revenue);

                double soldThisMonth = revenue / building->getPricePerSqm();
                std::cout << "Player " << id << " sold " << soldThisMonth << " sqm in cell "
                          << building->getCellIndex() << " for: +" << revenue
                          << " (total sold: " << building->getSoldArea() << "/" << building->getTotalArea()
                          << ")" << std::endl;
            } else {
                std::cout << "Player " << id << " house in cell " << building->getCellIndex()
                          << " generated 0 revenue this month" << std::endl;
            }
        }
    }
}

double Player::calculateHouseSales(Building* house, const QList<Player*>& allPlayers, int currentMonth) {
    // Проверяем, есть ли непроданная площадь
    double currentSoldArea = house->getSoldArea();
    double totalArea = house->getTotalArea();

    if (currentSoldArea >= totalArea) {
        return 0.0; // Все уже продано
    }

    Season currentSeason = getSeason(currentMonth);

    // Базовый спрос зависит от сезона
    double seasonalModifier = 1.0;
    switch(currentSeason) {
    case SPRING: seasonalModifier = GameConfig::SPRING_HOUSE_MODIFIER; break;
    case SUMMER: seasonalModifier = GameConfig::SUMMER_HOUSE_MODIFIER; break;
    case AUTUMN: seasonalModifier = GameConfig::AUTUMN_HOUSE_MODIFIER; break;
    case WINTER: seasonalModifier = GameConfig::WINTER_HOUSE_MODIFIER; break;
    }

    // Учитываем соседние магазины для увеличения спроса
    int neighborMarkets = countNeighborMarkets(house->getCellIndex(), allPlayers);
    double neighborBonus = 1.0 + (neighborMarkets * GameConfig::HOUSE_NEIGHBOR_BONUS);

    // Доступная для продажи площадь
    double availableArea = totalArea - currentSoldArea;

    // Базовый объем продаж в этом месяце (процент от оставшейся площади)
    double baseMonthlySales = availableArea * GameConfig::BASE_HOUSE_SALES_RATE;

    // Увеличиваем продажи за счет соседних магазинов и сезона
    double adjustedSales = baseMonthlySales * neighborBonus * seasonalModifier;

    // Учитываем влияние цены на спрос
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

    // Чем выше цена относительно базовой, тем меньше спрос
    double priceRatio = house->getPricePerSqm() / basePrice;
    double priceFactor = 1.0 - (priceRatio - 1.0) * GameConfig::PRICE_SENSITIVITY * 100;
    priceFactor = qMax(0.1, priceFactor); // Минимум 10% продаж даже при высокой цене

    // Финальный объем продаж в этом месяце
    double monthlySalesArea = adjustedSales * priceFactor;

    // Не можем продать больше, чем есть в наличии
    monthlySalesArea = qMin(monthlySalesArea, availableArea);

    // Не можем продать отрицательное количество
    monthlySalesArea = qMax(0.0, monthlySalesArea);

    // Обновляем проданную площадь
    house->setSoldArea(currentSoldArea + monthlySalesArea);

    // Выручка от продаж в этом месяце
    double revenue = monthlySalesArea * house->getPricePerSqm();

    // Отладочная информация
    std::cout << "House sales calculation for cell " << house->getCellIndex() << ":\n"
              << "  Available area: " << availableArea << "\n"
              << "  Base monthly sales: " << baseMonthlySales << "\n"
              << "  Neighbor bonus: " << neighborBonus << " (markets: " << neighborMarkets << ")\n"
              << "  Seasonal modifier: " << seasonalModifier << "\n"
              << "  Price factor: " << priceFactor << " (price ratio: " << priceRatio << ")\n"
              << "  Final monthly sales: " << monthlySalesArea << "\n"
              << "  Revenue: " << revenue << std::endl;

    return revenue;
}

void Player::receiveMarketRevenue(const QList<Player*>& allPlayers, int currentMonth) {
    Season currentSeason = getSeason(currentMonth);
    double baseRevenue = getMarketRevenue(currentSeason);

    for (int i = 0; i < buildings.size(); ++i) {
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
    for (int i = 0; i < buildings.size(); ++i) {
        Building* building = buildings[i];
        if ((building->getType() == Building::HOUSE_CONCRETE ||
             building->getType() == Building::HOUSE_WOOD ||
             building->getType() == Building::HOUSE_BRICK)) {

            double progress = 1.0;
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

            if (!building->getIsCompleted()) {
                // Для строящихся домов цена растет
                progress = static_cast<double>(building->getMonthsBuilt()) / building->getBuildTime();
            } else {
                // Для построенных домов цена медленно растет со временем
                int monthsCompleted = building->getMonthsBuilt() - building->getBuildTime();
                progress = 1.0 + (monthsCompleted * 0.01); // +1% за каждый месяц после завершения
            }

            double newPrice = basePrice * (1.0 + progress * 0.3);
            building->setPricePerSqm(newPrice);

            std::cout << "Updated price for house in cell " << building->getCellIndex()
                      << ": " << newPrice << " (progress: " << progress << ")" << std::endl;
        }
    }
}

// ... остальные методы без изменений ...

// Остальные методы без изменений (countNeighborHouses, countNeighborMarkets, getNeighborCells, getSeason, getHousingDemand, getMarketRevenue, hasBuildingInCell, calculateTotalCapital, getAllBuildings, getHouseCells, getMarketCells, canBuild, build, getLastMonthProfits, clearLastMonthProfits)

int Player::countNeighborHouses(int cellIndex, const QList<Player*>& allPlayers) const {
    int count = 0;
    QList<int> neighbors = getNeighborCells(cellIndex);

    for (int i = 0; i < neighbors.size(); ++i) {
        int neighborCell = neighbors[i];
        for (int j = 0; j < allPlayers.size(); ++j) {
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

    for (int i = 0; i < neighbors.size(); ++i) {
        int neighborCell = neighbors[i];
        for (int j = 0; j < allPlayers.size(); ++j) {
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
    for (int i = 0; i < buildings.size(); ++i) {
        if (buildings[i]->getCellIndex() == cellIndex) {
            return true;
        }
    }
    return false;
}

double Player::calculateTotalCapital() const {
    if (isBankrupt) return 0.0;

    double capital = money;

    for (int i = 0; i < buildings.size(); ++i) {
        Building* building = buildings[i];
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

QList<Player::BuildingInfo> Player::getAllBuildings() const {
    QList<BuildingInfo> allBuildings;
    for (int i = 0; i < buildings.size(); ++i) {
        Building* building = buildings[i];
        BuildingInfo info;
        info.cellIndex = building->getCellIndex();
        info.buildProgress = building->getMonthsBuilt();
        info.totalBuildTime = building->getBuildTime();
        info.isCompleted = building->getIsCompleted();
        info.type = building->getType();
        info.monthlyProfit = building->getMonthlyProfit();
        info.totalArea = building->getTotalArea();
        info.soldArea = building->getSoldArea();
        info.pricePerSqm = building->getPricePerSqm();
        allBuildings.append(info);
    }
    return allBuildings;
}

QList<int> Player::getHouseCells() const {
    QList<int> houseCells;
    for (int i = 0; i < buildings.size(); ++i) {
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
    for (int i = 0; i < buildings.size(); ++i) {
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
