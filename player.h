#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QColor>
#include <QVector>
#include <QList>
#include <QPair>
#include "building.h"

class Player {
public:
    // Структура для хранения информации о здании
    struct BuildingInfo {
        int cellIndex;
        int buildProgress; // Текущий этап строительства
        int totalBuildTime; // Всего этапов
        bool isCompleted;
        Building::Type type;
    };

    Player(QString playerName, int playerId, QColor playerColor);
    ~Player();

    // Геттеры
    QString getName() const { return name; }
    int getMoney() const { return money; }
    QColor getColor() const { return color; }
    int getId() const { return id; }
    QVector<Building*> getBuildings() const { return buildings; }

    // Управление деньгами
    bool changeMoney(int amount);
    void setMoney(int newMoney) { money = newMoney; }

    // Получение информации о зданиях
    QList<BuildingInfo> getHouseBuildings() const;
    QList<BuildingInfo> getMarketBuildings() const;
    QList<BuildingInfo> getAllBuildings() const;
    QList<int> getHouseCells() const;
    QList<int> getMarketCells() const;
    QList<int> getAllBuildingsCells() const;

    bool canBuild(Building::Type type);
    Building* build(Building::Type type, int cellIndex);
    void processMonth();
    int calculateTotalCapital();

private:
    QString name;
    int money;
    int id;
    QColor color;
    int advertisingHousing;
    int advertisingGoods;
    QVector<Building*> buildings;
};

#endif
