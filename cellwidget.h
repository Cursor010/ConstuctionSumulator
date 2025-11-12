#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class Building;

class CellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CellWidget(int index, QWidget *parent = nullptr);
    ~CellWidget();

    // Геттеры и сеттеры
    Building* getBuilding() const;
    void setBuilding(Building* newBuilding);
    bool isHighlighted() const;
    void setHighlighted(bool highlighted);
    QColor getHighlightColor() const;
    void setHighlightColor(const QColor& color);

    void updateCell();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void cellClicked(int cellIndex);

private:
    int cellIndex;
    Building* building;
    bool isHighlighted_;
    QColor highlightColor;
};

#endif
