#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include <cmath>
#include <random>
#include <ctime>

enum class BuildingType {
    NONE,
    PANEL_HOUSE,
    MONOLITH_HOUSE,
    BRICK_HOUSE,
    SUPERMARKET
};

enum class Season {
    WINTER,
    SPRING,
    SUMMER,
    AUTUMN
};

struct Building {
    BuildingType type = BuildingType::NONE;
    int cost = 0;
    int totalMonths = 0;
    int progress = 0;
    int ownerId = -1;
    int cellId = -1;
    bool isCompleted = false;

    // Для домов
    double totalArea = 0.0;
    double soldArea = 0.0;
    double pricePerSqm = 0.0;
    double advertisingBudget = 0.0;

    // Для супермаркетов
    double monthlyRevenue = 0.0;
    double marketAdvertisingBudget = 0.0;

    // Финансы строительства
    double totalSpent = 0.0;
    double monthlyPayment = 0.0;
};

class Player {
public:
    int id;
    double money;
    std::string name;
    std::vector<Building> buildings;
    std::vector<int> ownedCells;

    // Статистика
    double totalRevenue = 0.0;
    double totalConstructionCost = 0.0;
    double totalAdvertisingCost = 0.0;
    bool isBankrupt = false;

    Player(int id, const std::string& name, double startMoney)
        : id(id), name(name), money(startMoney) {}

    bool canAffordBuilding(const Building& building) const {
        return money >= building.monthlyPayment;
    }

    bool startBuilding(const Building& building) {
        if (!canAffordBuilding(building)) {
            return false;
        }

        Building newBuilding = building;
        newBuilding.monthlyPayment = static_cast<double>(newBuilding.cost) / newBuilding.totalMonths;

        // Платим только первый взнос
        double firstPayment = newBuilding.monthlyPayment;
        if (money >= firstPayment) {
            money -= firstPayment;
            newBuilding.totalSpent = firstPayment;
            buildings.push_back(newBuilding);
            totalConstructionCost += firstPayment;
            return true;
        }
        return false;
    }

    void processMonthlyPayments() {
        if (isBankrupt) return;

        // Сначала получаем доходы
        double monthlyIncome = 0.0;

        // Затем оплачиваем строительство
        for (auto& building : buildings) {
            if (!building.isCompleted) {
                if (money >= building.monthlyPayment) {
                    money -= building.monthlyPayment;
                    building.totalSpent += building.monthlyPayment;
                    building.progress++;

                    if (building.progress >= building.totalMonths) {
                        building.isCompleted = true;
                        std::cout << "Player " << id << " completed a building!" << std::endl;
                    }
                } else {
                    // Не хватает денег на этот месяц - строительство замедляется
                    // Но не останавливается полностью
                    if (money > building.monthlyPayment * 0.3) {
                        // Можем заплатить часть
                        double partialPayment = money * 0.5; // Оставляем немного на другие нужды
                        money -= partialPayment;
                        building.totalSpent += partialPayment;
                        building.progress += 0.5; // Половина прогресса
                        std::cout << "Player " << id << " made partial payment on building" << std::endl;
                    } else {
                        std::cout << "Player " << id << " cannot continue construction due to lack of funds" << std::endl;
                    }
                }
            }
        }

        // Проверка на банкротство
        if (money < 0) {
            isBankrupt = true;
            std::cout << "Player " << id << " is BANKRUPT!" << std::endl;
        }
    }

    double getTotalCapital() const {
        if (isBankrupt) return 0.0;

        double capital = money;

        for (const auto& building : buildings) {
            if (building.type == BuildingType::SUPERMARKET) {
                if (building.isCompleted) {
                    capital += building.cost * 1.6; // Стоимость на 60% выше
                } else {
                    // Стоимость незавершенного строительства
                    capital += building.totalSpent;
                }
            } else { // Дом
                if (building.isCompleted) {
                    double unsoldArea = building.totalArea - building.soldArea;
                    double costPerSqm = building.cost / building.totalArea;
                    capital += unsoldArea * costPerSqm; // Непроданное жилье по себестоимости
                    // Также добавляем стоимость проданной площади по себестоимости
                    // так как выручка уже в деньгах
                } else {
                    capital += building.totalSpent;
                }
            }
        }

        return capital;
    }

    void addRevenue(double amount) {
        if (!isBankrupt) {
            money += amount;
            totalRevenue += amount;
        }
    }

    int getConstructionCount() const {
        int count = 0;
        for (const auto& building : buildings) {
            if (!building.isCompleted) {
                count++;
            }
        }
        return count;
    }
};

class RealEstateAgency {
public:
    struct SaleResult {
        double soldArea = 0.0;
        double revenue = 0.0;
    };

    SaleResult processHousingSales(Player& player, Building& building,
        double baseDemand, double competitionModifier) {
        SaleResult result;

        if (player.isBankrupt) return result;

        if (building.isCompleted && building.totalArea > building.soldArea) {
            double availableArea = building.totalArea - building.soldArea;

            // Модификаторы спроса
            double demandModifier = 1.0;
            demandModifier += building.advertisingBudget * 0.005; // 0.5% за 1 тыс. у.е.
            demandModifier += competitionModifier * 0.1;

            double effectiveDemand = baseDemand * demandModifier;
            double priceModifier = 1.0 - (building.pricePerSqm - 8000) / 8000 * 0.1;

            double potentialSales = std::min(availableArea, effectiveDemand * priceModifier);
            result.soldArea = potentialSales;
            result.revenue = potentialSales * building.pricePerSqm;

            building.soldArea += result.soldArea;
            player.addRevenue(result.revenue);
        }

        return result;
    }

    double processSupermarketRevenue(Player& player, Building& building,
        double baseRevenue, double housingModifier) {
        if (player.isBankrupt || !building.isCompleted) return 0.0;

        double revenueModifier = 1.0;
        revenueModifier += building.marketAdvertisingBudget * 0.06; // 3% за 0.5 тыс. у.е.
        revenueModifier += housingModifier * 0.15;

        double revenue = baseRevenue * revenueModifier;
        player.addRevenue(revenue);

        return revenue;
    }
};

class Game {
private:
    std::vector<std::shared_ptr<Player>> players;
    std::map<int, Building> cellBuildings;
    int currentMonth = 0;
    int totalMonths = 12;
    const int BOARD_SIZE = 6;
    RealEstateAgency agency;

    const std::map<BuildingType, std::pair<int, int>> BUILDING_PARAMS = {
        {BuildingType::PANEL_HOUSE, {8000000, 7}},
        {BuildingType::MONOLITH_HOUSE, {12000000, 9}},
        {BuildingType::BRICK_HOUSE, {15000000, 12}},
        {BuildingType::SUPERMARKET, {2500000, 5}}
    };

    const std::map<BuildingType, double> HOUSE_AREAS = {
        {BuildingType::PANEL_HOUSE, 5000.0},
        {BuildingType::MONOLITH_HOUSE, 7000.0},
        {BuildingType::BRICK_HOUSE, 6000.0}
    };

    const double BASE_MARKET_REVENUE = 500000.0;

    std::mt19937 rng;

public:
    Game(int numPlayers, int months) : totalMonths(months) {
        std::random_device rd;
        rng.seed(rd());

        double startMoney = 37000000.0;
        for (int i = 0; i < numPlayers; ++i) {
            players.push_back(std::make_shared<Player>(i, "Player " + std::to_string(i), startMoney));
        }
    }

    Season getCurrentSeason() const {
        int monthInYear = currentMonth % 12;
        if (monthInYear >= 2 && monthInYear <= 4) return Season::SPRING;
        if (monthInYear >= 5 && monthInYear <= 7) return Season::SUMMER;
        if (monthInYear >= 8 && monthInYear <= 10) return Season::AUTUMN;
        return Season::WINTER;
    }

    double getHousingDemand() const {
        Season season = getCurrentSeason();
        double baseDemand = 1000.0;

        switch (season) {
        case Season::SPRING: return baseDemand * 1.2;
        case Season::SUMMER: return baseDemand * 1.1;
        case Season::AUTUMN: return baseDemand * 1.5;
        case Season::WINTER: return baseDemand * 0.8;
        }
        return baseDemand;
    }

    double getMarketRevenue() const {
        Season season = getCurrentSeason();
        double baseRevenue = BASE_MARKET_REVENUE;

        switch (season) {
        case Season::SPRING: return baseRevenue * 1.0;
        case Season::SUMMER: return baseRevenue * 1.1;
        case Season::AUTUMN: return baseRevenue * 1.3;
        case Season::WINTER: return baseRevenue * 1.6;
        }
        return baseRevenue;
    }

    bool isCellFree(int cellId) const {
        return cellBuildings.find(cellId) == cellBuildings.end();
    }

    void processConstruction(const std::vector<std::vector<int>>& constructionData) {
        for (int i = 0; i < players.size(); ++i) {
            if (players[i]->isBankrupt) continue;

            // Ограничиваем строительство одной постройкой за ход
            // Берем только первую ячейку из списка
            if (!constructionData[i].empty()) {
                int cellId = constructionData[i][0];

                if (isCellFree(cellId)) {
                    BuildingType type = (cellId % 3 == 0) ? BuildingType::SUPERMARKET :
                        (cellId % 3 == 1) ? BuildingType::PANEL_HOUSE : BuildingType::MONOLITH_HOUSE;

                    auto params = BUILDING_PARAMS.at(type);
                    Building newBuilding;
                    newBuilding.type = type;
                    newBuilding.cost = params.first;
                    newBuilding.totalMonths = params.second;
                    newBuilding.ownerId = i;
                    newBuilding.cellId = cellId;
                    newBuilding.progress = 0;

                    if (type != BuildingType::SUPERMARKET) {
                        newBuilding.totalArea = HOUSE_AREAS.at(type);
                        newBuilding.pricePerSqm = 10000.0 + (cellId % 5) * 1000.0;
                    } else {
                        newBuilding.monthlyRevenue = getMarketRevenue();
                    }

                    if (players[i]->startBuilding(newBuilding)) {
                        cellBuildings[cellId] = newBuilding;
                        players[i]->ownedCells.push_back(cellId);
                        std::cout << "Player " << i << " started building in cell " << cellId << std::endl;
                    } else {
                        std::cout << "Player " << i << " cannot afford building in cell " << cellId << std::endl;
                    }
                } else {
                    std::cout << "Player " << i << " cannot build in occupied cell " << cellId << std::endl;
                }
            }
        }
    }

    void processMonthlyOperations() {
        double baseHousingDemand = getHousingDemand();
        double baseMarketRevenue = getMarketRevenue();

        // Сначала все получают доход
        for (auto& player : players) {
            if (player->isBankrupt) continue;

            double competitionModifier = calculateCompetitionModifier(player->id);
            double housingModifier = calculateHousingModifier(player->id);

            for (auto& building : player->buildings) {
                if (building.type != BuildingType::SUPERMARKET) {
                    agency.processHousingSales(*player, building, baseHousingDemand, competitionModifier);
                } else {
                    agency.processSupermarketRevenue(*player, building, baseMarketRevenue, housingModifier);
                }
            }
        }

        // Затем все оплачивают строительство
        for (auto& player : players) {
            player->processMonthlyPayments();

            // Обновляем состояние в cellBuildings
            for (auto& building : player->buildings) {
                if (cellBuildings.find(building.cellId) != cellBuildings.end()) {
                    cellBuildings[building.cellId] = building;
                }
            }

            // Автоматическое увеличение цены в ходе строительства
            for (auto& building : player->buildings) {
                if (!building.isCompleted && building.type != BuildingType::SUPERMARKET) {
                    double progressRatio = static_cast<double>(building.progress) / building.totalMonths;
                    building.pricePerSqm = 8000.0 + 4000.0 * progressRatio;
                }
            }
        }
    }

    void processMonth(const std::vector<std::vector<int>>& constructionData) {
        std::cout << "\n=== Month " << currentMonth + 1 << " ===" << std::endl;

        processConstruction(constructionData);
        processMonthlyOperations();

        currentMonth++;
        printGameState();
    }

    double calculateCompetitionModifier(int playerId) const {
        double modifier = 0.0;
        for (const auto& player : players) {
            if (player->id == playerId || player->isBankrupt) continue;

            for (const auto& building : player->buildings) {
                if (building.type == BuildingType::SUPERMARKET && building.isCompleted) {
                    modifier += 0.1;
                }
            }
        }
        return modifier;
    }

    double calculateHousingModifier(int playerId) const {
        double modifier = 0.0;
        for (const auto& player : players) {
            if (player->id == playerId || player->isBankrupt) continue;

            for (const auto& building : player->buildings) {
                if (building.type != BuildingType::SUPERMARKET && building.isCompleted) {
                    modifier += 0.08;
                }
            }
        }
        return modifier;
    }

    void printGameState() const {
        Season season = getCurrentSeason();
        std::string seasonStr;
        switch (season) {
        case Season::WINTER: seasonStr = "Winter"; break;
        case Season::SPRING: seasonStr = "Spring"; break;
        case Season::SUMMER: seasonStr = "Summer"; break;
        case Season::AUTUMN: seasonStr = "Autumn"; break;
        }

        std::cout << "Season: " << seasonStr << std::endl;
        std::cout << "Housing demand: " << getHousingDemand() << " sqm" << std::endl;
        std::cout << "Market revenue base: " << getMarketRevenue() << std::endl;

        for (const auto& player : players) {
            std::cout << "\nPlayer " << player->id << " (" << player->name << "):" << std::endl;
            std::cout << "  Money: " << player->money << std::endl;
            std::cout << "  Total Capital: " << player->getTotalCapital() << std::endl;
            std::cout << "  Total Revenue: " << player->totalRevenue << std::endl;
            if (player->isBankrupt) {
                std::cout << "  STATUS: BANKRUPT" << std::endl;
                continue;
            }

            int houses = 0, markets = 0, underConstruction = 0;
            for (const auto& building : player->buildings) {
                if (building.type != BuildingType::SUPERMARKET) {
                    if (building.isCompleted) houses++;
                    else underConstruction++;
                } else {
                    if (building.isCompleted) markets++;
                    else underConstruction++;
                }
            }

            std::cout << "  Buildings - Houses: " << houses << ", Markets: " << markets
                << ", Under construction: " << underConstruction << std::endl;
        }

        // Вывод карты
        std::cout << "\nGame Board:" << std::endl;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                int cellId = i * BOARD_SIZE + j;
                auto it = cellBuildings.find(cellId);
                if (it != cellBuildings.end()) {
                    char symbol = '?';
                    switch (it->second.type) {
                    case BuildingType::PANEL_HOUSE: symbol = it->second.isCompleted ? 'P' : 'p'; break;
                    case BuildingType::MONOLITH_HOUSE: symbol = it->second.isCompleted ? 'M' : 'm'; break;
                    case BuildingType::BRICK_HOUSE: symbol = it->second.isCompleted ? 'B' : 'b'; break;
                    case BuildingType::SUPERMARKET: symbol = it->second.isCompleted ? 'S' : 's'; break;
                    default: symbol = '?';
                    }
                    std::cout << symbol << it->second.ownerId << " ";
                } else {
                    std::cout << ".  ";
                }
            }
            std::cout << std::endl;
        }
    }

    std::shared_ptr<Player> getWinner() const {
        auto winner = std::max_element(players.begin(), players.end(),
            [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
                return a->getTotalCapital() < b->getTotalCapital();
            });
        return *winner;
    }

    bool isGameOver() const {
        return currentMonth >= totalMonths;
    }

    const std::vector<std::shared_ptr<Player>>& getPlayers() const { return players; }
    int getCurrentMonth() const { return currentMonth; }
    int getTotalMonths() const { return totalMonths; }
    const std::map<int, Building>& getCellBuildings() const { return cellBuildings; }
};

class AIPlayer {
public:
    static std::vector<int> getConstructionStrategy(const Game& game, int playerId) {
        std::vector<int> constructionCells;
        const auto& players = game.getPlayers();

        if (playerId >= players.size() || players[playerId]->isBankrupt) {
            return constructionCells;
        }

        // Стратегия: строить только одну постройку за ход
        // Сначала проверяем, есть ли уже строящиеся объекты
        int constructionCount = players[playerId]->getConstructionCount();
        if (constructionCount > 0) {
            // Уже есть строящиеся объекты, не строим новый в этот ход
            return constructionCells;
        }

        double availableMoney = players[playerId]->money;

        // Собираем список свободных клеток
        std::vector<int> freeCells;
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                int cellId = i * 6 + j;
                if (game.isCellFree(cellId)) {
                    freeCells.push_back(cellId);
                }
            }
        }

        if (freeCells.empty()) {
            return constructionCells;
        }

        // Выбираем случайную свободную клетку с вероятностью 40%
        if (rand() % 100 < 40 && availableMoney > 3000000) {
            int randomIndex = rand() % freeCells.size();
            constructionCells.push_back(freeCells[randomIndex]);
            std::cout << "AI Player " << playerId << " decided to build in cell " << freeCells[randomIndex] << std::endl;
        }

        return constructionCells;
    }
};

int main() {
    srand(time(0));
    int numPlayers = 3;
    int totalMonths = 18;

    Game game(numPlayers, totalMonths);

    std::cout << "=== REAL ESTATE INVESTMENT GAME ===" << std::endl;
    std::cout << "Players: " << numPlayers << ", Months: " << totalMonths << std::endl;
    std::cout << "AI players can build only ONE building per turn" << std::endl;

    while (!game.isGameOver()) {
        std::vector<std::vector<int>> constructionData;

        for (int i = 0; i < numPlayers; ++i) {
            constructionData.push_back(AIPlayer::getConstructionStrategy(game, i));
        }

        game.processMonth(constructionData);

        std::cout << "\nPress Enter to continue to next month...";
        std::cin.get();
    }

    auto winner = game.getWinner();
    std::cout << "\n=== GAME OVER ===" << std::endl;
    std::cout << "Winner: Player " << winner->id << " (" << winner->name << ")" << std::endl;
    std::cout << "Total Capital: " << winner->getTotalCapital() << std::endl;

    // Итоговая статистика
    std::cout << "\nFinal Statistics:" << std::endl;
    for (const auto& player : game.getPlayers()) {
        std::cout << "Player " << player->id << ": " << player->getTotalCapital()
            << (player->isBankrupt ? " (BANKRUPT)" : "") << std::endl;
    }

    return 0;
}