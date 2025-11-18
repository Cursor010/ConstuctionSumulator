#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QColor>
#include <QList>
#include <QPair>
#include "building.h"
#include "gameconfig.h"

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
        double totalArea;
        double soldArea;
        double pricePerSqm;
    };

    Player(QString playerName, int playerId, QColor playerColor);
    ~Player();

    // Основные методы согласно требованиям
    void processMonth(const QList<Player*>& allPlayers, int currentMonth);

    // Методы для строительства
    bool canBuild(Building::Type type);
    Building* build(Building::Type type, int cellIndex);

    // Расчет капитала
    double calculateTotalCapital() const;

    // Геттеры
    QString getName() const { return name; }
    double getMoney() const { return money; }
    QColor getColor() const { return color; }
    int getId() const { return id; }
    bool getIsBankrupt() const { return isBankrupt; }

    void addMoney(double amount) { money += amount; }
    void subtractMoney(double amount) { money -= amount; }

    // Методы для работы со зданиями
    QList<BuildingInfo> getAllBuildings() const;
    QList<int> getHouseCells() const;
    QList<int> getMarketCells() const;
    QList<Building*> getBuildings() const { return buildings; }

    // Методы для получения прибыли
    QList<QPair<int, double>> getLastMonthProfits() const;
    void clearLastMonthProfits();

    // Вспомогательные методы
    Season getSeason(int month) const;
    double getHousingDemand(Season season) const;
    double getMarketRevenue(Season season) const;

    // Методы для подсчета соседей (сделаны публичными)
    int countNeighborHouses(int cellIndex, const QList<Player*>& allPlayers) const;
    int countNeighborMarkets(int cellIndex, const QList<Player*>& allPlayers) const;

private:
    // Операции месяца согласно требованиям
    void payConstructionCosts();                                   // п.1
    void receiveMarketRevenue(const QList<Player*>& allPlayers, int currentMonth);   // п.3
    void updateHousingPrices();                                    // п.4
    void sellHousing(const QList<Player*>& allPlayers, int currentMonth);            // п.2 - продажа квартир

    // Вспомогательные методы
    QList<int> getNeighborCells(int cellIndex) const;
    bool hasBuildingInCell(int cellIndex) const;
    double calculateHouseSales(Building* house, const QList<Player*>& allPlayers, int currentMonth);

    QString name;
    double money;  // в у.е.
    int id;
    QColor color;
    bool isBankrupt;

    QList<Building*> buildings;
    QList<QPair<int, double>> lastMonthProfits;
};

#endif // PLAYER_H
