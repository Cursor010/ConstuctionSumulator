#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "building.h"

class CellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellWidget(int index, QWidget *parent = nullptr);
    ~CellWidget();

    Building* getBuilding() const;
    void setBuilding(Building* newBuilding);
    bool isHighlighted() const;
    void setHighlighted(bool highlighted);
    QColor getHighlightColor() const;
    void setHighlightColor(const QColor& color);

    // Метод для отображения прибыли
    void showProfit(double profit);

    // Убрал updateCell(), используем стандартный update()

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void hideProfit();

signals:
    void cellClicked(int index);

private:
    int cellIndex;
    Building* building;
    bool isHighlighted_;
    QColor highlightColor;

    // Для отображения прибыли
    QLabel* profitLabel;
    QTimer* profitTimer;
};

#endif
