#include "building.h"

Building::Building(Type t, int owner, int buildTime, int buildingCost, QColor color, int cellIndex)
    : type(t), ownerId(owner), buildTime(buildTime), cost(buildingCost),
    ownerColor(color), cellIndex(cellIndex)
{
    monthsBuilt = 0;
    isCompleted = false;
    totalArea = 0;
    soldArea = 0;
    pricePerSqm = 0;
    monthlyProfit = 0;
}

void Building::progressMonth() {
    if (!isCompleted) {
        monthsBuilt++;
        if (monthsBuilt >= buildTime) {
            isCompleted = true;
        }
    }
}
