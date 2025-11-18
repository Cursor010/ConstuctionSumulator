#ifndef BUILDING_H
#define BUILDING_H

#include <QColor>

class Building
{
public:
    enum Type {
        NO_BUILDING = 0,
        HOUSE_CONCRETE = 1,
        HOUSE_WOOD = 2,
        HOUSE_BRICK = 3,
        MARKET = 4
    };

    Building(Type t, int owner, int buildTime, double buildingCost, QColor color, int cellIndex);

    // Геттеры
    Type getType() const { return type; }
    int getOwnerId() const { return ownerId; }
    int getBuildTime() const { return buildTime; }
    double getCost() const { return cost; }
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
    void addToMonthlyProfit(double amount) { monthlyProfit += amount; }

    void progressMonth();

private:
    Type type;
    int ownerId;
    int buildTime;
    double cost;
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
