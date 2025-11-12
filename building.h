#ifndef BUILDING_H
#define BUILDING_H

#include <QColor>

class Building {
public:
    enum Type {
        NO_BUILDING,
        HOUSE,
        MARKET
    };

    Building(Type t, int owner, int buildTime, int buildingCost, QColor color);

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
};

#endif
