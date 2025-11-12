#include "cellwidget.h"
#include "building.h"

CellWidget::CellWidget(int index, QWidget *parent)
    : QWidget(parent), cellIndex(index), building(nullptr), isHighlighted_(false)
{
    setFixedSize(100, 100);
}

CellWidget::~CellWidget() {
    // Не удаляем building здесь, так как им управляет Player
}

Building* CellWidget::getBuilding() const {
    return building;
}

void CellWidget::setBuilding(Building* newBuilding) {
    building = newBuilding;
}

bool CellWidget::isHighlighted() const {
    return isHighlighted_;
}

void CellWidget::setHighlighted(bool highlighted) {
    isHighlighted_ = highlighted;
}

QColor CellWidget::getHighlightColor() const {
    return highlightColor;
}

void CellWidget::setHighlightColor(const QColor& color) {
    highlightColor = color;
}

void CellWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем границу
    painter.setPen(Qt::black);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    // Если есть здание
    if (building) {
        QColor buildingColor = building->getOwnerColor();

        if (!building->getIsCompleted()) {
            // Для незавершенного строительства
            if (building->getType() == Building::HOUSE) {
                // Дом - квадрат
                painter.setBrush(buildingColor);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawRect(rect().adjusted(15, 15, -15, -15));

                // Отображаем этап строительства
                painter.setPen(Qt::white);
                QString progressText = QString("%1/%2").arg(building->getMonthsBuilt()).arg(building->getBuildTime());
                painter.drawText(rect(), Qt::AlignCenter, progressText);
            } else {
                // Магазин - круг
                painter.setBrush(buildingColor);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawEllipse(rect().adjusted(15, 15, -15, -15));

                // Отображаем этап строительства
                painter.setPen(Qt::white);
                QString progressText = QString("%1/%2").arg(building->getMonthsBuilt()).arg(building->getBuildTime());
                painter.drawText(rect(), Qt::AlignCenter, progressText);
            }
        } else {
            // Для завершенного строительства
            if (building->getType() == Building::HOUSE) {
                // Дом - квадрат
                painter.setBrush(buildingColor);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawRect(rect().adjusted(15, 15, -15, -15));

                // Отображаем текст "Дом"
                painter.setPen(Qt::white);
                painter.drawText(rect(), Qt::AlignCenter, "Дом");
            } else {
                // Магазин - круг
                painter.setBrush(buildingColor);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawEllipse(rect().adjusted(15, 15, -15, -15));

                // Отображаем текст "Магазин"
                painter.setPen(Qt::white);
                painter.drawText(rect(), Qt::AlignCenter, "Магазин");
            }
        }
    }

    // Отображаем номер клетки
    painter.setPen(Qt::gray);
    painter.drawText(rect().adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignTop, QString::number(cellIndex));
}

void CellWidget::mousePressEvent(QMouseEvent* event)
{
    emit cellClicked(cellIndex);
}

void CellWidget::updateCell()
{
    update();
}
