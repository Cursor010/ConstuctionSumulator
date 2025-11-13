#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QColor>
#include <QVector>
#include <QList>
#include "building.h"

// Перечисление сезонов
enum Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

class Player {
public:
    // Структура для хранения информации о здании
    struct BuildingInfo {
        int cellIndex;
        int buildProgress;
        int totalBuildTime;
        bool isCompleted;
        Building::Type type;
        double monthlyProfit; // Прибыль за месяц
    };

    Player(QString playerName, int playerId, QColor playerColor);
    ~Player();

    // Геттеры
    QString getName() const { return name; }
    int getMoney() const { return money; }
    QColor getColor() const { return color; }
    int getId() const { return id; }
    QVector<Building*> getBuildings() const { return buildings; }
    bool getIsBankrupt() const { return isBankrupt; }
    double getTotalRevenue() const { return totalRevenue; }
    double getTotalConstructionCost() const { return totalConstructionCost; }
    double getTotalAdvertisingCost() const { return totalAdvertisingCost; }

    // Основной метод обновления состояния
    void updateState(const QList<int>& newHouseCells, const QList<int>& newMarketCells, int currentMonth);

    // Методы для получения информации о зданиях
    QList<BuildingInfo> getHouseBuildings() const;
    QList<BuildingInfo> getMarketBuildings() const;
    QList<BuildingInfo> getAllBuildings() const;
    QList<int> getHouseCells() const;
    QList<int> getMarketCells() const;
    QList<int> getAllBuildingsCells() const;

    // Методы строительства
    bool canBuild(Building::Type type);
    Building* build(Building::Type type, int cellIndex);

    // Метод обработки месяца (для обратной совместимости)
    void processMonth();

    // Расчет общего капитала
    double calculateTotalCapital() const;

    // Получение прибыли по зданиям
    QList<QPair<int, double>> getBuildingsMonthlyProfit() const;

    // Методы для работы с прибылью за последний месяц
    void setLastMonthProfits(const QList<QPair<int, double>>& profits);
    QList<QPair<int, double>> getLastMonthProfits() const;
    void clearLastMonthProfits();

private:
    // Константы игры
    static const int INITIAL_MONEY = 37000000;
    static const int HOUSE_BUILD_TIME = 6;
    static const int HOUSE_COST = 8000000;
    static const int MARKET_BUILD_TIME = 5;
    static const int MARKET_COST = 2500000;
    static const double BASE_HOUSE_PRICE;
    static const int BASE_HOUSE_DEMAND = 1000;
    static const int BASE_MARKET_REVENUE = 500000;

    // Основные поля
    QString name;
    int money;
    int id;
    QColor color;
    bool isBankrupt;

    // Статистика
    double totalRevenue;
    double totalConstructionCost;
    double totalAdvertisingCost;

    // Списки зданий
    QVector<Building*> buildings;

    // Текущие и предыдущие позиции зданий
    QList<int> currentHouseCells;
    QList<int> currentMarketCells;
    QList<int> previousHouseCells;
    QList<int> previousMarketCells;

    // Прибыль за последний месяц
    QList<QPair<int, double>> lastMonthProfits;

    // Вспомогательные методы
    void processConstruction(const QList<int>& newHouseCells, const QList<int>& newMarketCells);
    void processMonthlyOperations(Season season);
    void processHousingSales(Building* house, double baseDemand);
    void processMarketRevenue(Building* market, double baseRevenue);
    Season getSeason(int month) const;
    double getHousingDemand(Season season) const;
    double getMarketRevenue(Season season) const;
    bool hasBuildingInCell(int cellIndex) const;
};

#endif
