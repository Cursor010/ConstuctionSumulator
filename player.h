#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QColor>
#include <QList>
#include <QPair>
#include "building.h"

class Player {
public:
    struct BuildingInfo {
        int cellIndex;
        int buildProgress;
        int totalBuildTime;
        bool isCompleted;
        Building::Type type;
        double monthlyProfit;
    };

    Player(QString playerName, int playerId, QColor playerColor);
    ~Player();

    // Статические константы
    static const int INITIAL_MONEY = 37000000;
    static const int HOUSE_COST = 8000000;
    static const int MARKET_COST = 2500000;
    static const int HOUSE_BUILD_TIME = 6;
    static const int MARKET_BUILD_TIME = 5;
    static const double BASE_HOUSE_PRICE;

    // Геттеры
    QString getName() const { return name; }
    double getMoney() const { return money; }
    int getId() const { return id; }
    QColor getColor() const { return color; }
    bool getIsBankrupt() const { return isBankrupt; }

    // Методы для зданий
    bool canBuild(Building::Type type);
    Building* build(Building::Type type, int cellIndex);

    QList<BuildingInfo> getHouseBuildings() const;
    QList<BuildingInfo> getMarketBuildings() const;
    QList<BuildingInfo> getAllBuildings() const;

    QList<int> getHouseCells() const;
    QList<int> getMarketCells() const;
    QList<int> getAllBuildingsCells() const;

    // Финансовые методы
    void processMonth();
    double calculateTotalCapital() const;

    // Прибыль
    QList<QPair<int, double>> getLastMonthProfits() const;
    void setLastMonthProfits(const QList<QPair<int, double>>& profits);
    void clearLastMonthProfits();
    QList<QPair<int, double>> getBuildingsMonthlyProfit() const;

private:
    enum class Season { SPRING, SUMMER, AUTUMN, WINTER };

    void updateState(const QList<int>& newHouseCells, const QList<int>& newMarketCells, int currentMonth);
    void processConstruction(const QList<int>& newHouseCells, const QList<int>& newMarketCells);
    void processMonthlyOperations(Season season);
    void processHousingSales(Building* house, double baseDemand);
    void processMarketRevenue(Building* market, double baseRevenue);

    Season getSeason(int month) const;
    double getHousingDemand(Season season) const;
    double getMarketRevenue(Season season) const;
    bool hasBuildingInCell(int cellIndex) const;

    QString name;
    double money;
    int id;
    QColor color;
    bool isBankrupt;

    QList<Building*> buildings;
    QList<int> currentHouseCells;
    QList<int> currentMarketCells;
    QList<int> previousHouseCells;
    QList<int> previousMarketCells;

    double totalRevenue;
    double totalConstructionCost;
    double totalAdvertisingCost;

    QList<QPair<int, double>> lastMonthProfits;

    // Константы для расчета доходов
    static const int BASE_HOUSE_DEMAND = 1000;
    static const int BASE_MARKET_REVENUE = 500000;
};

#endif // PLAYER_H
