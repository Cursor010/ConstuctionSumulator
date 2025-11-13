#include "building.h"

Building::Building(Type t, int owner, int buildTime, int buildingCost, QColor color, int cellIndex) {
    type = t;
    ownerId = owner;
    this->buildTime = buildTime;
    monthsBuilt = 1;
    cost = buildingCost;
    isCompleted = (buildTime == 1);
    constructionCostPerMonth = cost / buildTime;
    totalArea = 0.0;
    soldArea = 0.0;
    pricePerSqm = 0.0;
    monthlyProfit = 0.0;
    ownerColor = color;
    this->cellIndex = cellIndex;

    if (buildTime == 1) {
        isCompleted = true;
    }
}

void Building::progressMonth() {
    if (!isCompleted) {
        monthsBuilt++;
        if (monthsBuilt >= buildTime) {
            isCompleted = true;
        }
    }
}
