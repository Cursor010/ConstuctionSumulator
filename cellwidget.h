#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "building.h"

class Building;

class CellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellWidget(int index, QWidget *parent = nullptr);
    ~CellWidget();

    Building* getBuilding() const;
    void setBuilding(Building* newBuilding);

    void showProfit(double profit);

signals:
    void cellClicked(int cellIndex);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void hideProfit();

private:
    int cellIndex;
    Building* building;

    QLabel* profitLabel;
    QTimer* profitTimer;
};

#endif // CELLWIDGET_H
