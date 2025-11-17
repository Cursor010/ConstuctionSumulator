#ifndef BUILDING_H
#define BUILDING_H

#include <QColor>

class Building
{
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
    int getCost() const { return cost; }
    QColor getOwnerColor() const { return ownerColor; }
    int getCellIndex() const { return cellIndex; }
    int getMonthsBuilt() const { return monthsBuilt; }
    bool getIsCompleted() const { return isCompleted; }
    double getTotalArea() const { return totalArea; }
    double getSoldArea() const { return soldArea; }
    double getPricePerSqm() const { return pricePerSqm; }
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
    int cost;
    QColor ownerColor;
    int cellIndex;

    int monthsBuilt;
    bool isCompleted;
    double totalArea;
    double soldArea;
    double pricePerSqm;
    double monthlyProfit;
};

#endif // BUILDING_H
