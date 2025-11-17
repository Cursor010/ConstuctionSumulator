#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QColor>
#include <QList>
#include <QPair>
#include "building.h"

class Player
{
public:
    enum Season {
        SPRING,
        SUMMER,
        AUTUMN,
        WINTER
    };

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

    // Константы
    static const int INITIAL_MONEY = 150000;
    static const int HOUSE_COST = 50000;
    static const int MARKET_COST = 80000;
    static const int HOUSE_BUILD_TIME = 6;
    static const int MARKET_BUILD_TIME = 5;
    static const double BASE_HOUSE_DEMAND;
    static const double BASE_MARKET_REVENUE;
    static const double BASE_HOUSE_PRICE;

    // Основные методы
    void updateState(const QList<int>& newHouseCells, const QList<int>& newMarketCells, int currentMonth);
    void processMonth();
    bool canBuild(Building::Type type);
    Building* build(Building::Type type, int cellIndex);
    double calculateTotalCapital() const;

    // Геттеры
    QString getName() const { return name; }
    int getMoney() const { return money; }
    QColor getColor() const { return color; }
    int getId() const { return id; }
    bool getIsBankrupt() const { return isBankrupt; }

    // Методы для работы со зданиями
    QList<BuildingInfo> getHouseBuildings() const;
    QList<BuildingInfo> getMarketBuildings() const;
    QList<BuildingInfo> getAllBuildings() const;
    QList<int> getHouseCells() const;
    QList<int> getMarketCells() const;
    QList<int> getAllBuildingsCells() const;

    // Методы для получения прибыли
    QList<QPair<int, double>> getLastMonthProfits() const;
    void setLastMonthProfits(const QList<QPair<int, double>>& profits);
    void clearLastMonthProfits();

private:
    void processConstruction(const QList<int>& newHouseCells, const QList<int>& newMarketCells);
    void processMonthlyOperations(Season season);
    void processHousingSales(Building* house, double baseDemand);
    void processMarketRevenue(Building* market, double baseRevenue);
    Season getSeason(int month) const;
    double getHousingDemand(Season season) const;
    double getMarketRevenue(Season season) const;
    bool hasBuildingInCell(int cellIndex) const;

    QString name;
    int money;
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
};

#endif // PLAYER_H
