#ifndef BUILDING_H
#define BUILDING_H

#include <QColor>

<<<<<<< HEAD
=======
const int HOUSE_COST = 8000000;
const int MARKET_COST = 2500000;
const int HOUSE_BUILD_TIME = 6;
const int MARKET_BUILD_TIME = 3;
const int INITIAL_MONEY = 37000000;

>>>>>>> front
class Building {
public:
    enum Type {
        NO_BUILDING,
        HOUSE,
        MARKET
    };

<<<<<<< HEAD
    Building(Type t, int owner, int buildTime, int buildingCost, QColor color);
=======
    Building(Type t, int owner, int buildTime, int buildingCost, QColor color, int cellIndex);
>>>>>>> front

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
<<<<<<< HEAD
=======
    int getCellIndex() const { return cellIndex; }
>>>>>>> front

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
<<<<<<< HEAD
=======
    int cellIndex;
>>>>>>> front
};

#endif
