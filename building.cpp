#include "building.h"

Building::Building(Type t, int owner, int buildTime, int buildingCost, QColor color) {
    type = t;
    ownerId = owner;
    this->buildTime = buildTime;
    monthsBuilt = 0;
    cost = buildingCost;
    isCompleted = false;
    constructionCostPerMonth = cost / buildTime;
    totalApartments = (t == HOUSE) ? 100 : 0;
    soldApartments = 0;
    ownerColor = color;
}

void Building::progressMonth() {
    if (!isCompleted) {
        monthsBuilt++;
        if (monthsBuilt >= buildTime) {
            isCompleted = true;
        }
    }
}
