#ifndef BUILDING_H
#define BUILDING_H

#include <QColor>

// Константы строительства
const int HOUSE_CONCRETE_COST = 8000000;
const int HOUSE_WOOD_COST = 8000000;
const int HOUSE_BRICK_COST = 8000000;
const int MARKET_COST = 2500000;
const int HOUSE_BUILD_TIME = 6;
const int MARKET_BUILD_TIME = 5;
const int INITIAL_MONEY = 37000000;

// Константы для расчета доходов
const int BASE_HOUSE_DEMAND = 1000;
const int BASE_MARKET_REVENUE = 500000;
const double BASE_HOUSE_PRICE = 10000.0;

class Building {
public:
    enum Type {
        NO_BUILDING,
        HOUSE_CONCRETE,
        HOUSE_WOOD,
        HOUSE_BRICK,
        MARKET
    };

    Building(Type t, int owner, int buildTime, int buildingCost, QColor color, int cellIndex);

    // Геттеры
    Type getType() const { return type; }
    int getOwnerId() const { return ownerId; }
    int getBuildTime() const { return buildTime; }
    int getMonthsBuilt() const { return monthsBuilt; }
    int getCost() const { return cost; }
    bool getIsCompleted() const { return isCompleted; }
    int getCellIndex() const { return cellIndex; }
    double getTotalArea() const { return totalArea; }
    double getSoldArea() const { return soldArea; }
    double getPricePerSqm() const { return pricePerSqm; }
    QColor getOwnerColor() const { return ownerColor; }
    double getMonthlyProfit() const { return monthlyProfit; }

    // Сеттеры
    void setTotalArea(double area) { totalArea = area; }
    void setSoldArea(double area) { soldArea = area; }
    void setPricePerSqm(double price) { pricePerSqm = price; }
    void setMonthlyProfit(double profit) { monthlyProfit = profit; }

    void progressMonth();

private:
    Type type;
    int ownerId;
    int buildTime;
    int monthsBuilt;
    int cost;
    bool isCompleted;
    double totalArea;
    double soldArea;
    double pricePerSqm;
    double monthlyProfit;
    QColor ownerColor;
    int cellIndex;
};

#endif
