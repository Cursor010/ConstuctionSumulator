#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QColor>
#include <QList>
#include "building.h"

class Player
{
public:
    enum Season { SPRING, SUMMER, AUTUMN, WINTER };

    struct BuildingInfo {
        int cellIndex;
        int buildProgress;
        int totalBuildTime;
        bool isCompleted;
        Building::Type type;
        double monthlyProfit;
        double totalArea;
        double soldArea;
        double pricePerSqm;
        double housingAdBudget;
        double marketAdBudget;
    };

    Player(QString playerName, int playerId, QColor playerColor);
    ~Player();

    QString getName() const { return name; }
    double getMoney() const { return money; }
    QColor getColor() const { return color; }
    bool getIsBankrupt() const { return isBankrupt; }

    void addMoney(double amount) { money += amount; }
    void setHousingAdBudget(double budget) { housingAdBudget = budget; }
    void setMarketAdBudget(double budget) { marketAdBudget = budget; }

    bool canBuild(Building::Type buildingType);
    Building* build(Building::Type type, int cellIndex);
    void processMonth(const QList<Player*>& allPlayers, int currentMonth);
    void updateAdBudgets();

    Season getSeason(int month) const;
    double getHousingDemand(Season season) const;
    double getMarketRevenue(Season season) const;

    bool hasBuildingInCell(int cellIndex) const;
    QList<Building*> getBuildings() const { return buildings; }
    QList<BuildingInfo> getAllBuildings() const;
    QList<int> getHouseCells() const;
    QList<int> getMarketCells() const;

    int countNeighborHouses(int cellIndex, const QList<Player*>& allPlayers) const;
    int countNeighborMarkets(int cellIndex, const QList<Player*>& allPlayers) const;

    double calculateTotalCapital() const;

    QList<QPair<int, double>> getLastMonthProfits() const { return lastMonthProfits; }
    void clearLastMonthProfits() { lastMonthProfits.clear(); }

private:
    QString name;
    double money;
    int id;
    QColor color;
    bool isBankrupt;
    double housingAdBudget;
    double marketAdBudget;
    double lastHousingAdBudget;
    QList<Building*> buildings;
    QList<QPair<int, double>> lastMonthProfits;

    void payConstructionCosts();
    void sellHousing(const QList<Player*>& allPlayers, int currentMonth);
    double calculateHouseSales(Building* house, const QList<Player*>& allPlayers, int currentMonth);
    void receiveMarketRevenue(const QList<Player*>& allPlayers, int currentMonth);
    void updateHousingPrices();
    QList<int> getNeighborCells(int cellIndex) const;
};

#endif // PLAYER_H
