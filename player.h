#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QColor>
#include <QVector>
#include "building.h"

class Player {
public:
    Player(QString playerName, int playerId, QColor playerColor);
    ~Player();

    // Геттеры
    QString getName() const { return name; }
    int getMoney() const { return money; }
    QColor getColor() const { return color; }
    int getId() const { return id; }
    QVector<Building*> getBuildings() const { return buildings; }

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
