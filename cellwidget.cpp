#include "cellwidget.h"
#include "building.h"
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

CellWidget::CellWidget(int index, QWidget *parent)
    : QWidget(parent), cellIndex(index), building(nullptr), isHighlighted_(false)
{
    setFixedSize(100, 100);

    // Инициализация для отображения прибыли
    profitLabel = new QLabel(this);
    profitLabel->setAlignment(Qt::AlignCenter);
    profitLabel->setStyleSheet("QLabel { background-color: rgba(255, 255, 255, 200); border: 1px solid gray; font-weight: bold; font-size: 10px; }");
    profitLabel->setFixedSize(60, 20);
    profitLabel->hide();

    profitTimer = new QTimer(this);
    profitTimer->setSingleShot(true);
    connect(profitTimer, &QTimer::timeout, this, &CellWidget::hideProfit);
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

// Метод для отображения прибыли
void CellWidget::showProfit(double profit) {
    if (profit == 0) return; // Не показываем нулевую прибыль

    QString text = QString::number(profit, 'f', 0);
    if (profit > 0) {
        text = "+" + text;
        profitLabel->setStyleSheet("QLabel { color: green; background-color: rgba(255, 255, 255, 200); border: 1px solid gray; font-weight: bold; font-size: 10px; }");
    } else {
        profitLabel->setStyleSheet("QLabel { color: red; background-color: rgba(255, 255, 255, 200); border: 1px solid gray; font-weight: bold; font-size: 10px; }");
    }

    profitLabel->setText(text);
    profitLabel->move((width() - profitLabel->width()) / 2, 5);
    profitLabel->show();

    // Скрыть через 1.5 секунды
    profitTimer->start(1500);
}

void CellWidget::hideProfit() {
    profitLabel->hide();
}

void CellWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем фон
    if (isHighlighted_) {
        painter.fillRect(rect(), QBrush(highlightColor.lighter(150)));
    } else {
        painter.fillRect(rect(), QBrush(Qt::lightGray));
    }

    // Рисуем границу
    painter.setPen(Qt::black);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    // Если есть здание
    if (building) {
        QColor ownerColor = building->getOwnerColor();

        // Рисуем подложку цвета игрока внизу клетки
        QRect footerRect(5, height() - 15, width() - 10, 10);
        painter.fillRect(footerRect, ownerColor);

        if (building->getType() == Building::HOUSE) {
            // Загружаем текстуру дома
            QPixmap houseTexture("D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/home.PNG");
            if (!houseTexture.isNull()) {
                // Масштабируем текстуру под размер клетки
                QRect imageRect(0, 10, width(), height() - 25);
                painter.drawPixmap(imageRect, houseTexture);
            } else {
                // Fallback: рисуем квадрат если текстура не загрузилась
                painter.setBrush(ownerColor);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawRect(15, 15, width() - 30, height() - 45);
            }

            // Отображаем этап строительства выше изображения
            if (!building->getIsCompleted()) {
                painter.setPen(Qt::black);
                QString progressText = QString("%1/%2").arg(building->getMonthsBuilt()).arg(building->getBuildTime());
                painter.drawText(QRect(0, height() - 30, width(), 15), Qt::AlignCenter, progressText);
            } else {
                painter.setPen(Qt::darkGreen);
                painter.drawText(QRect(0, height() - 30, width(), 15), Qt::AlignCenter, "✅");
            }

        } else if (building->getType() == Building::MARKET) {
            // Загружаем текстуру магазина
            QPixmap marketTexture("D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/shop.PNG");
            if (!marketTexture.isNull()) {
                // Масштабируем текстуру под размер клетки
                QRect imageRect(0, 10, width(), height() - 25);
                painter.drawPixmap(imageRect, marketTexture);
            } else {
                // Fallback: рисуем круг если текстура не загрузилась
                painter.setBrush(ownerColor);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawEllipse(15, 15, width() - 30, height() - 45);
            }

            // Отображаем этап строительства выше изображения
            if (!building->getIsCompleted()) {
                painter.setPen(Qt::black);
                QString progressText = QString("%1/%2").arg(building->getMonthsBuilt()).arg(building->getBuildTime());
                painter.drawText(QRect(0, height() - 30, width(), 15), Qt::AlignCenter, progressText);
            } else {
                painter.setPen(Qt::darkGreen);
                painter.drawText(QRect(0, height() - 30, width(), 15), Qt::AlignCenter, "✅");
            }
        }
    }

    // Отображаем номер клетки в левом верхнем углу
    painter.setPen(Qt::gray);
    painter.drawText(rect().adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignTop, QString::number(cellIndex));
}

void CellWidget::mousePressEvent(QMouseEvent* event)
{
    emit cellClicked(cellIndex);
}
