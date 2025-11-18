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
    int getId() const { return id; }
    bool getIsBankrupt() const { return isBankrupt; }

    void processMonth(const QList<Player*>& allPlayers, int currentMonth);
    bool canBuild(Building::Type type);
    Building* build(Building::Type type, int cellIndex);
    bool hasBuildingInCell(int cellIndex) const;
    double calculateTotalCapital() const;
    QList<BuildingInfo> getAllBuildings() const;
    QList<int> getHouseCells() const;
    QList<int> getMarketCells() const;
    QList<QPair<int, double>> getLastMonthProfits() const;
    void clearLastMonthProfits();
    void addMoney(double amount) { money += amount; }

    // Рекламные методы
    void setHousingAdBudget(double budget) { housingAdBudget = budget; }
    void setMarketAdBudget(double budget) { marketAdBudget = budget; }
    double getHousingAdBudget() const { return housingAdBudget; }
    double getMarketAdBudget() const { return marketAdBudget; }
    double getLastHousingAdBudget() const { return lastHousingAdBudget; }
    void updateAdBudgets();

    // Вспомогательные методы
    Season getSeason(int month) const;
    double getHousingDemand(Season season) const;
    double getMarketRevenue(Season season) const;
    int countNeighborHouses(int cellIndex, const QList<Player*>& allPlayers) const;
    int countNeighborMarkets(int cellIndex, const QList<Player*>& allPlayers) const;
    QList<Building*> getBuildings() const { return buildings; }

private:
    void payConstructionCosts();
    void sellHousing(const QList<Player*>& allPlayers, int currentMonth);
    double calculateHouseSales(Building* house, const QList<Player*>& allPlayers, int currentMonth);
    void receiveMarketRevenue(const QList<Player*>& allPlayers, int currentMonth);
    void updateHousingPrices();
    QList<int> getNeighborCells(int cellIndex) const;

    QString name;
    double money;
    int id;
    QColor color;
    bool isBankrupt;
    QList<Building*> buildings;
    QList<QPair<int, double>> lastMonthProfits;

    // Рекламные бюджеты (в тыс. у.е.)
    double housingAdBudget;
    double marketAdBudget;
    double lastHousingAdBudget;
};

#endif // PLAYER_H
