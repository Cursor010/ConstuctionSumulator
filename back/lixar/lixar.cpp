#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <cmath>

enum class BuildingType {
    PANEL_HOUSE,
    MONOLITH_HOUSE,
    BRICK_HOUSE,
    SUPERMARKET,
    NONE
};

struct MonthlyReport {
    int month;
    double income;
    double expenses;
    double profit;

    MonthlyReport(int m, double inc = 0, double exp = 0)
        : month(m), income(inc), expenses(exp), profit(inc - exp) {}
};

struct Building {
    BuildingType type;
    int playerId;
    int x, y;
    int constructionTimeLeft;
    int totalConstructionTime;
    double totalCost;
    double investedMoney;
    int constructionStartMonth;
    double monthlyPayment;

    // Финансовая статистика
    double totalIncome;
    double totalExpenses;
    std::vector<MonthlyReport> monthlyReports;

    Building(BuildingType t, int pId, int posX, int posY, int time, double cost, int startMonth)
        : type(t), playerId(pId), x(posX), y(posY),
        constructionTimeLeft(time), totalConstructionTime(time), totalCost(cost), investedMoney(0),
        constructionStartMonth(startMonth), totalIncome(0), totalExpenses(0) {
        monthlyPayment = totalCost / totalConstructionTime;
    }

    void addMonthlyReport(int month, double income, double expenses) {
        monthlyReports.emplace_back(month, income, expenses);
        totalIncome += income;
        totalExpenses += expenses;
    }

    double getTotalProfit() const {
        return totalIncome - totalExpenses;
    }
};

class Player {
private:
    int id;
    double money;
    std::vector<std::shared_ptr<Building>> buildings;
    double totalIncome;
    double totalExpenses;

public:
    Player(int playerId, double startMoney) : id(playerId), money(startMoney),
        totalIncome(0), totalExpenses(0) {}

    void addBuilding(std::shared_ptr<Building> building) {
        buildings.push_back(building);
    }

    void addMoney(double amount) {
        money += amount;
        if (amount > 0) totalIncome += amount;
        else totalExpenses -= amount;
    }

    // Геттеры
    int getId() const { return id; }
    double getMoney() const { return money; }
    const std::vector<std::shared_ptr<Building>>& getBuildings() const { return buildings; }
    double getTotalIncome() const { return totalIncome; }
    double getTotalExpenses() const { return totalExpenses; }
    double getNetProfit() const { return totalIncome - totalExpenses; }

    // Подсчет общего капитала по правилам игры
    double calculateTotalCapital() const {
        double capital = money;

        for (const auto& building : buildings) {
            if (building->constructionTimeLeft > 0) {
                capital += building->investedMoney;
            } else {
                if (building->type == BuildingType::SUPERMARKET) {
                    capital += building->totalCost * 1.6;
                } else {
                    capital += building->totalCost;
                }
            }
        }
        return capital;
    }
};

class GameEngine {
private:
    std::vector<Player> players;
    std::vector<std::vector<std::shared_ptr<Building>>> board;
    int currentMonth;
    int totalMonths;
    int boardSize;
    bool gameFinished;

    std::map<BuildingType, std::pair<int, double>> buildingParams = {
        {BuildingType::PANEL_HOUSE, {7, 8.0}},
        {BuildingType::MONOLITH_HOUSE, {10, 12.0}},
        {BuildingType::BRICK_HOUSE, {12, 15.0}},
        {BuildingType::SUPERMARKET, {5, 2.5}}
    };

public:
    GameEngine(int numPlayers, int months) : boardSize(6), currentMonth(0),
        totalMonths(months), gameFinished(false) {
        board.resize(boardSize, std::vector<std::shared_ptr<Building>>(boardSize));

        for (int i = 0; i < numPlayers; ++i) {
            players.emplace_back(i, 37.0);
        }
    }

    bool processTurn(int playerId, BuildingType buildingType, int x, int y) {
        if (gameFinished) return false;
        if (playerId < 0 || playerId >= players.size()) return false;
        if (x < 0 || x >= boardSize || y < 0 || y >= boardSize) return false;
        if (board[x][y] != nullptr) {
            std::cout << "Клетка уже занята!" << std::endl;
            return false;
        }

        Player& player = players[playerId];
        auto& params = buildingParams[buildingType];
        double cost = params.second;
        int time = params.first;

        double firstPayment = cost / time;
        if (player.getMoney() < firstPayment) {
            std::cout << "Недостаточно денег для начала строительства! Нужно для первого платежа: "
                << firstPayment << " млн, есть: " << player.getMoney() << " млн" << std::endl;
            return false;
        }

        auto newBuilding = std::make_shared<Building>(
            buildingType, playerId, x, y, time, cost, currentMonth
            );

        player.addMoney(-firstPayment);
        newBuilding->investedMoney = firstPayment;

        board[x][y] = newBuilding;
        player.addBuilding(newBuilding);

        std::cout << "Игрок " << playerId << " начал строительство " << buildingTypeToString(buildingType)
            << " в клетке (" << x << "," << y << "). Первый платеж: " << firstPayment << " млн" << std::endl;

        return true;
    }

    void simulateMonth() {
        if (gameFinished || currentMonth >= totalMonths) {
            gameFinished = true;
            return;
        }

        std::cout << "\n=== Месяц " << currentMonth + 1 << " ===" << std::endl;

        processConstructionCosts();
        calculateHousingIncome();
        calculateSupermarketProfit();

        currentMonth++;

        if (currentMonth >= totalMonths) {
            gameFinished = true;
            determineWinner();
        }
    }

private:
    void processConstructionCosts() {
        for (auto& player : players) {
            for (auto& building : player.getBuildings()) {
                if (building->constructionTimeLeft > 0) {
                    double monthlyCost = building->monthlyPayment;

                    if (player.getMoney() >= monthlyCost) {
                        player.addMoney(-monthlyCost);
                        building->investedMoney += monthlyCost;
                        building->constructionTimeLeft--;

                        // Запись расходов в отчет
                        building->addMonthlyReport(currentMonth, 0, monthlyCost);

                        if (building->constructionTimeLeft == 0) {
                            std::cout << "Игрок " << player.getId() << " завершил строительство "
                                << buildingTypeToString(building->type)
                                << " в клетке (" << building->x << "," << building->y
                                << "). Всего инвестировано: " << building->investedMoney << " млн" << std::endl;
                        }
                    } else {
                        std::cout << "Игрок " << player.getId() << " не может внести платеж " << monthlyCost
                            << " млн за " << buildingTypeToString(building->type)
                            << ". Строительство приостановлено." << std::endl;
                    }
                }
            }
        }
    }

    void calculateHousingIncome() {
        double baseDemand = getSeasonalDemand();

        // Считаем общее количество супермаркетов на доске
        int totalSupermarkets = countTotalBuildings(BuildingType::SUPERMARKET);
        double supermarketDensityBonus = calculateSupermarketDensityBonus(totalSupermarkets);

        for (auto& player : players) {
            for (auto& building : player.getBuildings()) {
                if (building->constructionTimeLeft == 0) {
                    if (building->type == BuildingType::PANEL_HOUSE ||
                        building->type == BuildingType::MONOLITH_HOUSE ||
                        building->type == BuildingType::BRICK_HOUSE) {

                        double localSupermarketBonus = calculateLocalSupermarketBonus(building->x, building->y);
                        double income = baseDemand * (1.0 + localSupermarketBonus) *
                            getHouseMultiplier(building->type) * supermarketDensityBonus;

                        player.addMoney(income);

                        // Запись дохода в отчет здания
                        building->addMonthlyReport(currentMonth, income, 0);

                        std::cout << "Дом " << buildingTypeToString(building->type)
                            << " игрока " << player.getId() << " в (" << building->x << "," << building->y
                            << ") принес: " << income << " млн (бонус от супермаркетов: "
                            << (localSupermarketBonus * 100) << "%, глобальный бонус: "
                            << ((supermarketDensityBonus - 1.0) * 100) << "%)" << std::endl;
                    }
                }
            }
        }
    }

    void calculateSupermarketProfit() {
        double baseSales = getSeasonalSales();

        // Считаем общее количество домов на доске
        int totalHouses = countTotalBuildings(BuildingType::PANEL_HOUSE) +
            countTotalBuildings(BuildingType::MONOLITH_HOUSE) +
            countTotalBuildings(BuildingType::BRICK_HOUSE);
        double housingDensityBonus = calculateHousingDensityBonus(totalHouses);

        for (auto& player : players) {
            for (auto& building : player.getBuildings()) {
                if (building->constructionTimeLeft == 0 &&
                    building->type == BuildingType::SUPERMARKET) {

                    double localHousingBonus = calculateLocalHousingBonus(building->x, building->y);
                    double profit = baseSales * (1.0 + localHousingBonus) * housingDensityBonus;

                    player.addMoney(profit);

                    // Запись дохода в отчет здания
                    building->addMonthlyReport(currentMonth, profit, 0);

                    std::cout << "Супермаркет игрока " << player.getId() << " в (" << building->x << "," << building->y
                        << ") принес: " << profit << " млн (бонус от домов: "
                        << (localHousingBonus * 100) << "%, глобальный бонус: "
                        << ((housingDensityBonus - 1.0) * 100) << "%)" << std::endl;
                }
            }
        }
    }

    // Подсчет общего количества зданий определенного типа
    int countTotalBuildings(BuildingType type) const {
        int count = 0;
        for (const auto& row : board) {
            for (const auto& cell : row) {
                if (cell && cell->type == type && cell->constructionTimeLeft == 0) {
                    count++;
                }
            }
        }
        return count;
    }

    // Бонус от плотности супермаркетов (глобальный эффект)
    double calculateSupermarketDensityBonus(int totalSupermarkets) const {
        // Чем больше супермаркетов, тем выше спрос на жилье (до определенного предела)
        // Максимальный бонус +100% при 10+ супермаркетах
        double bonus = 1.0 + (std::min(totalSupermarkets, 10) * 0.1);
        return bonus;
    }

    // Бонус от плотности домов (глобальный эффект)
    double calculateHousingDensityBonus(int totalHouses) const {
        // Чем больше домов, тем выше спрос в супермаркетах (до определенного предела)
        // Максимальный бонус +150% при 15+ домах
        double bonus = 1.0 + (std::min(totalHouses, 15) * 0.1);
        return bonus;
    }

    // Локальный бонус от супермаркетов для домов
    double calculateLocalSupermarketBonus(int x, int y) const {
        double bonus = 0.0;
        // Проверяем клетки в радиусе 2
        for (int dx = -2; dx <= 2; ++dx) {
            for (int dy = -2; dy <= 2; ++dy) {
                if (dx == 0 && dy == 0) continue;

                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize) {
                    if (board[nx][ny] && board[nx][ny]->type == BuildingType::SUPERMARKET) {
                        // Бонус уменьшается с расстоянием
                        double distance = std::sqrt(dx * dx + dy * dy);
                        double distanceMultiplier = 1.0 / distance;
                        bonus += 0.2 * distanceMultiplier;
                    }
                }
            }
        }
        // Ограничиваем максимальный локальный бонус +80%
        return std::min(bonus, 0.8);
    }

    // Локальный бонус от домов для супермаркетов
    double calculateLocalHousingBonus(int x, int y) const {
        double bonus = 0.0;
        // Проверяем клетки в радиусе 2
        for (int dx = -2; dx <= 2; ++dx) {
            for (int dy = -2; dy <= 2; ++dy) {
                if (dx == 0 && dy == 0) continue;

                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize) {
                    if (board[nx][ny] &&
                        (board[nx][ny]->type == BuildingType::PANEL_HOUSE ||
                            board[nx][ny]->type == BuildingType::MONOLITH_HOUSE ||
                            board[nx][ny]->type == BuildingType::BRICK_HOUSE)) {
                        // Бонус уменьшается с расстоянием
                        double distance = std::sqrt(dx * dx + dy * dy);
                        double distanceMultiplier = 1.0 / distance;
                        bonus += 0.25 * distanceMultiplier;
                    }
                }
            }
        }
        // Ограничиваем максимальный локальный бонус +100%
        return std::min(bonus, 1.0);
    }

    double getSeasonalDemand() const {
        int season = currentMonth % 12;
        if (season >= 8 && season <= 10) return 1.5;
        if (season >= 2 && season <= 4) return 1.2;
        return 1.0;
    }

    double getSeasonalSales() const {
        int season = currentMonth % 12;
        if (season >= 11 || season <= 1) return 1.6;
        if (season >= 8 && season <= 10) return 1.3;
        return 1.0;
    }

    double getHouseMultiplier(BuildingType type) const {
        switch (type) {
        case BuildingType::PANEL_HOUSE: return 0.8;
        case BuildingType::MONOLITH_HOUSE: return 1.0;
        case BuildingType::BRICK_HOUSE: return 1.2;
        default: return 0.0;
        }
    }

    void determineWinner() {
        std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
        std::cout << "Финальные результаты:" << std::endl;

        int winnerId = -1;
        double maxCapital = -1;

        for (const auto& player : players) {
            double capital = player.calculateTotalCapital();
            std::cout << "Игрок " << player.getId() << ": " << std::fixed << std::setprecision(2)
                << capital << " млн (доход: " << player.getTotalIncome()
                << " млн, расходы: " << player.getTotalExpenses() << " млн)" << std::endl;

            if (capital > maxCapital) {
                maxCapital = capital;
                winnerId = player.getId();
            }
        }

        std::cout << "\nПОБЕДИТЕЛЬ: Игрок " << winnerId << " с капиталом "
            << maxCapital << " млн!" << std::endl;

        // Детальный отчет по всем зданиям
        printDetailedBuildingReport();

        // Статистика по плотности застройки
        printDensityStatistics();
    }

    void printDensityStatistics() const {
        std::cout << "\n=== СТАТИСТИКА ПЛОТНОСТИ ЗАСТРОЙКИ ===" << std::endl;

        int totalHouses = countTotalBuildings(BuildingType::PANEL_HOUSE) +
            countTotalBuildings(BuildingType::MONOLITH_HOUSE) +
            countTotalBuildings(BuildingType::BRICK_HOUSE);
        int totalSupermarkets = countTotalBuildings(BuildingType::SUPERMARKET);

        double houseDensityBonus = calculateHousingDensityBonus(totalHouses);
        double supermarketDensityBonus = calculateSupermarketDensityBonus(totalSupermarkets);

        std::cout << "Всего домов на доске: " << totalHouses << std::endl;
        std::cout << "Всего супермаркетов на доске: " << totalSupermarkets << std::endl;
        std::cout << "Глобальный бонус для супермаркетов от домов: +"
            << ((houseDensityBonus - 1.0) * 100) << "%" << std::endl;
        std::cout << "Глобальный бонус для домов от супермаркетов: +"
            << ((supermarketDensityBonus - 1.0) * 100) << "%" << std::endl;
    }

    void printDetailedBuildingReport() const {
        std::cout << "\n=== ДЕТАЛЬНЫЙ ОТЧЕТ ПО ВСЕМ ЗДАНИЯМ ===" << std::endl;

        for (const auto& player : players) {
            std::cout << "\n--- Игрок " << player.getId() << " ---" << std::endl;

            if (player.getBuildings().empty()) {
                std::cout << "Нет построек" << std::endl;
                continue;
            }

            for (const auto& building : player.getBuildings()) {
                std::cout << "\n" << buildingTypeToString(building->type)
                    << " в (" << building->x << "," << building->y << "):" << std::endl;
                std::cout << "  Общие инвестиции: " << building->investedMoney << " млн" << std::endl;
                std::cout << "  Общий доход: " << building->totalIncome << " млн" << std::endl;
                std::cout << "  Общие расходы: " << building->totalExpenses << " млн" << std::endl;
                std::cout << "  Чистая прибыль: " << building->getTotalProfit() << " млн" << std::endl;

                // Месячный отчет
                if (!building->monthlyReports.empty()) {
                    std::cout << "  Помесячная статистика:" << std::endl;
                    for (const auto& report : building->monthlyReports) {
                        std::cout << "    Месяц " << report.month + 1 << ": "
                            << "доход=" << report.income << " млн, "
                            << "расходы=" << report.expenses << " млн, "
                            << "прибыль=" << report.profit << " млн" << std::endl;
                    }
                }
            }
        }
    }

public:
    const std::vector<Player>& getPlayers() const { return players; }
    const std::vector<std::vector<std::shared_ptr<Building>>>& getBoard() const { return board; }
    int getCurrentMonth() const { return currentMonth; }
    int getTotalMonths() const { return totalMonths; }
    bool isGameFinished() const { return gameFinished; }

    static std::string buildingTypeToString(BuildingType type) {
        switch (type) {
        case BuildingType::PANEL_HOUSE: return "Панельный дом";
        case BuildingType::MONOLITH_HOUSE: return "Монолитный дом";
        case BuildingType::BRICK_HOUSE: return "Кирпичный дом";
        case BuildingType::SUPERMARKET: return "Супермаркет";
        default: return "Неизвестно";
        }
    }

    void printGameState() const {
        std::cout << "\n=== Состояние игры ===" << std::endl;
        std::cout << "Месяц: " << currentMonth + 1 << " из " << totalMonths << std::endl;

        // Статистика по плотности
        int totalHouses = countTotalBuildings(BuildingType::PANEL_HOUSE) +
            countTotalBuildings(BuildingType::MONOLITH_HOUSE) +
            countTotalBuildings(BuildingType::BRICK_HOUSE);
        int totalSupermarkets = countTotalBuildings(BuildingType::SUPERMARKET);

        std::cout << "Всего домов на доске: " << totalHouses << std::endl;
        std::cout << "Всего супермаркетов на доске: " << totalSupermarkets << std::endl;

        for (const auto& player : players) {
            std::cout << "Игрок " << player.getId() << ": " << std::fixed << std::setprecision(2)
                << player.getMoney() << " млн (капитал: " << player.calculateTotalCapital()
                << " млн, построек: " << player.getBuildings().size() << ")" << std::endl;
        }

        std::cout << "\nДоска:" << std::endl;
        for (int x = 0; x < boardSize; ++x) {
            for (int y = 0; y < boardSize; ++y) {
                if (board[x][y]) {
                    char buildingChar = '?';
                    switch (board[x][y]->type) {
                    case BuildingType::PANEL_HOUSE: buildingChar = 'P'; break;
                    case BuildingType::MONOLITH_HOUSE: buildingChar = 'M'; break;
                    case BuildingType::BRICK_HOUSE: buildingChar = 'B'; break;
                    case BuildingType::SUPERMARKET: buildingChar = 'S'; break;
                    default: buildingChar = '?';
                    }
                    std::cout << buildingChar << board[x][y]->playerId << " ";
                } else {
                    std::cout << ".  ";
                }
            }
            std::cout << std::endl;
        }

        // Краткий финансовый отчет по зданиям
        std::cout << "\nКраткий финансовый отчет по зданиям:" << std::endl;
        for (const auto& player : players) {
            for (const auto& building : player.getBuildings()) {
                std::string status = (building->constructionTimeLeft > 0) ?
                    "строится (" + std::to_string(building->constructionTimeLeft) + " мес.)" : "построен";
                std::cout << "Игрок " << player.getId() << ": "
                    << buildingTypeToString(building->type) << " в ("
                    << building->x << "," << building->y << ") - " << status
                    << ", инвестировано: " << building->investedMoney << " млн"
                    << ", прибыль: " << building->getTotalProfit() << " млн" << std::endl;
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "");
    std::cout << "=== ИНВЕСТИЦИИ В СТРОИТЕЛЬСТВО ===" << std::endl;
    std::cout << "Теперь с УСИЛЕННОЙ зависимостью цен от количества супермаркетов и домов!" << std::endl;
    std::cout << "Чем больше супермаркетов - тем выше спрос на жилье!" << std::endl;
    std::cout << "Чем больше домов - тем выше спрос в супермаркетах!" << std::endl;

    int numPlayers, totalMonths;
    std::cout << "Количество игроков (2-5): ";
    std::cin >> numPlayers;
    std::cout << "Количество месяцев (6-24): ";
    std::cin >> totalMonths;

    GameEngine game(numPlayers, totalMonths);

    while (!game.isGameFinished()) {
        game.printGameState();

        for (int playerId = 0; playerId < numPlayers; ++playerId) {
            if (game.isGameFinished()) break;

            std::cout << "\n--- Ход игрока " << playerId << " ---" << std::endl;
            std::cout << "Выберите действие:" << std::endl;
            std::cout << "1 - Построить панельный дом (8 млн, 7 мес., 1.14 млн/мес.)" << std::endl;
            std::cout << "2 - Построить монолитный дом (12 млн, 10 мес., 1.2 млн/мес.)" << std::endl;
            std::cout << "3 - Построить кирпичный дом (15 млн, 12 мес., 1.25 млн/мес.)" << std::endl;
            std::cout << "4 - Построить супермаркет (2.5 млн, 5 мес., 0.5 млн/мес.)" << std::endl;
            std::cout << "0 - Пропустить ход" << std::endl;

            int choice, x, y;
            std::cin >> choice;

            if (choice >= 1 && choice <= 4) {
                std::cout << "Введите координаты (x y от 0 до 5): ";
                std::cin >> x >> y;

                BuildingType type;
                switch (choice) {
                case 1: type = BuildingType::PANEL_HOUSE; break;
                case 2: type = BuildingType::MONOLITH_HOUSE; break;
                case 3: type = BuildingType::BRICK_HOUSE; break;
                case 4: type = BuildingType::SUPERMARKET; break;
                default: type = BuildingType::PANEL_HOUSE;
                }

                if (!game.processTurn(playerId, type, x, y)) {
                    std::cout << "Не удалось выполнить ход. Попробуйте снова." << std::endl;
                    playerId--;
                }
            } else {
                std::cout << "Игрок " << playerId << " пропускает ход" << std::endl;
            }
        }

        if (!game.isGameFinished()) {
            game.simulateMonth();

            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.ignore();
            std::cin.get();
        }
    }

    return 0;
}