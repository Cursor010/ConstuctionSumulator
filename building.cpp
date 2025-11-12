#include "building.h"

<<<<<<< HEAD
Building::Building(Type t, int owner, int buildTime, int buildingCost, QColor color) {
    type = t;
    ownerId = owner;
    this->buildTime = buildTime;
    monthsBuilt = 0;
    cost = buildingCost;
    isCompleted = false;
=======
Building::Building(Type t, int owner, int buildTime, int buildingCost, QColor color, int cellIndex) {
    type = t;
    ownerId = owner;
    this->buildTime = buildTime;
    monthsBuilt = 1; // Начинаем с первого этапа, а не с нулевого
    cost = buildingCost;
    isCompleted = (buildTime == 1); // Если время постройки 1 месяц, здание сразу завершено
>>>>>>> front
    constructionCostPerMonth = cost / buildTime;
    totalApartments = (t == HOUSE) ? 100 : 0;
    soldApartments = 0;
    ownerColor = color;
<<<<<<< HEAD
=======
    this->cellIndex = cellIndex;

    // Если время постройки 1 месяц, здание сразу завершено
    if (buildTime == 1) {
        isCompleted = true;
    }
>>>>>>> front
}

void Building::progressMonth() {
    if (!isCompleted) {
        monthsBuilt++;
        if (monthsBuilt >= buildTime) {
            isCompleted = true;
        }
    }
}
