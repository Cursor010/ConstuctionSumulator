#ifndef BUILDING_H
#define BUILDING_H

#include <QColor>

const int HOUSE_COST = 8000000;
const int MARKET_COST = 2500000;
const int HOUSE_BUILD_TIME = 6;
const int MARKET_BUILD_TIME = 3;
const int INITIAL_MONEY = 37000000;

class Building {
public:
    enum Type {
        NO_BUILDING,
        HOUSE,
        MARKET
    };

    Building(Type t, int owner, int buildTime, int buildingCost, QColor color, int cellIndex);

    // Геттеры
    Type getType() const { return type; }
    int getOwnerId() const { return ownerId; }
    int getMonthsBuilt() const { return monthsBuilt; }
    int getBuildTime() const { return buildTime; }
    bool getIsCompleted() const { return isCompleted; }
    QColor getOwnerColor() const { return ownerColor; }
    int getConstructionCostPerMonth() const { return constructionCostPerMonth; }
    int getCost() const { return cost; }
    int getTotalApartments() const { return totalApartments; }
    int getSoldApartments() const { return soldApartments; }
    int getCellIndex() const { return cellIndex; }

    // Сеттеры
    void setSoldApartments(int sold) { soldApartments = sold; }
    void setMonthsBuilt(int months) { monthsBuilt = months; }
    void setIsCompleted(bool completed) { isCompleted = completed; }

    void progressMonth();

private:
    Type type;
    int ownerId;
    int buildTime;
    int monthsBuilt;
    int cost;
    bool isCompleted;
    int constructionCostPerMonth;
    int totalApartments;
    int soldApartments;
    QColor ownerColor;
    int cellIndex;
};

#endif
