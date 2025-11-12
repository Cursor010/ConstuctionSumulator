#include "building.h"

Building::Building(Type t, int owner, int buildTime, int buildingCost, QColor color, int cellIndex) {
    type = t;
    ownerId = owner;
    this->buildTime = buildTime;
    monthsBuilt = 1; // Начинаем с первого этапа, а не с нулевого
    cost = buildingCost;
    isCompleted = (buildTime == 1); // Если время постройки 1 месяц, здание сразу завершено
    constructionCostPerMonth = cost / buildTime;
    totalApartments = (t == HOUSE) ? 100 : 0;
    soldApartments = 0;
    ownerColor = color;
    this->cellIndex = cellIndex;

    // Если время постройки 1 месяц, здание сразу завершено
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
