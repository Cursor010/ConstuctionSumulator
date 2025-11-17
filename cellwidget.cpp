#include "cellwidget.h"
#include "building.h"
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

CellWidget::CellWidget(int index, QWidget *parent)
    : QWidget(parent), cellIndex(index), building(nullptr)
{
    setFixedSize(100, 100);

    // Инициализация для отображения прибыли
    profitLabel = new QLabel(this);
    profitLabel->setAlignment(Qt::AlignCenter);
    profitLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 10px; }");
    profitLabel->setFixedSize(60, 16);
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

void CellWidget::showProfit(double profit) {
    if (profit == 0) return;

    QString text = QString::number(profit, 'f', 0);
    if (profit > 0) {
        text = "+" + text;
        profitLabel->setStyleSheet("QLabel { color: green; font-weight: bold; font-size: 10px; }");
    } else {
        profitLabel->setStyleSheet("QLabel { color: red; font-weight: bold; font-size: 10px; }");
    }

    profitLabel->setText(text);
    profitLabel->move((width() - profitLabel->width()) / 2, 2);
    profitLabel->show();
    profitTimer->start(1500);
}

void CellWidget::hideProfit() {
    profitLabel->hide();
}

void CellWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем границу клетки
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    // Если есть здание
    if (building) {
        QColor ownerColor = building->getOwnerColor();

        // Область для изображения здания (уменьшенная, чтобы оставить место для текста прибыли)
        QRect imageRect(15, 20, width() - 30, height() - 45);

        // Область для подложки цвета игрока (компактная)
        QRect footerRect(5, height() - 22, width() - 10, 12);

        // Область для текста прогресса строительства
        QRect progressRect(5, height() - 22, width() - 10, 12);

        if (building->getType() == Building::HOUSE_CONCRETE ||
            building->getType() == Building::HOUSE_WOOD ||
            building->getType() == Building::HOUSE_BRICK) {

            // Определяем текстуру в зависимости от типа дома
            QString texturePath;
            if (building->getType() == Building::HOUSE_CONCRETE) {
                texturePath = "D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/homeConcrete.jpg";
            } else if (building->getType() == Building::HOUSE_WOOD) {
                texturePath = "D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/homeWood.jpg";
            } else if (building->getType() == Building::HOUSE_BRICK) {
                texturePath = "D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/homeBrick.jpg";
            }

            QPixmap houseTexture(texturePath);
            if (!houseTexture.isNull()) {
                // Масштабируем с сохранением пропорций и центрируем
                QPixmap scaledTexture = houseTexture.scaled(imageRect.width(), imageRect.height(),
                                                            Qt::KeepAspectRatio, Qt::SmoothTransformation);

                // Вычисляем позицию для центрирования
                int x = imageRect.x() + (imageRect.width() - scaledTexture.width()) / 2;
                int y = imageRect.y() + (imageRect.height() - scaledTexture.height()) / 2;

                painter.drawPixmap(x, y, scaledTexture);
            } else {
                // Fallback: рисуем стилизованный дом если текстура не загрузилась
                painter.setBrush(ownerColor.lighter(150));
                painter.setPen(QPen(Qt::black, 1));

                // Основание дома (уменьшенное)
                painter.drawRect(imageRect.x() + 5, imageRect.y() + 10, imageRect.width() - 10, imageRect.height() - 20);

                // Крыша
                QPolygon roof;
                roof << QPoint(imageRect.x() + 2, imageRect.y() + 10)
                     << QPoint(imageRect.x() + imageRect.width() - 2, imageRect.y() + 10)
                     << QPoint(imageRect.x() + imageRect.width() / 2, imageRect.y() + 2);
                painter.drawPolygon(roof);
            }

            // Рисуем компактную подложку цвета игрока
            painter.setBrush(ownerColor);
            painter.setPen(QPen(Qt::black, 1));
            painter.drawRect(footerRect);

            // Отображаем этап строительства на подложке
            if (!building->getIsCompleted()) {
                painter.setPen(Qt::white);
                QFont progressFont = painter.font();
                progressFont.setPointSize(7);
                progressFont.setBold(true);
                painter.setFont(progressFont);

                QString progressText = QString("%1/%2").arg(building->getMonthsBuilt()).arg(building->getBuildTime());
                painter.drawText(progressRect, Qt::AlignCenter, progressText);
            } else {
                painter.setPen(Qt::white);
                QFont progressFont = painter.font();
                progressFont.setPointSize(8);
                progressFont.setBold(true);
                painter.setFont(progressFont);
                painter.drawText(progressRect, Qt::AlignCenter, "✔");
            }

        } else if (building->getType() == Building::MARKET) {
            QPixmap marketTexture("D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/shop.PNG");
            if (!marketTexture.isNull()) {
                // Масштабируем с сохранением пропорций и центрируем
                QPixmap scaledTexture = marketTexture.scaled(imageRect.width(), imageRect.height(),
                                                             Qt::KeepAspectRatio, Qt::SmoothTransformation);

                // Вычисляем позицию для центрирования
                int x = imageRect.x() + (imageRect.width() - scaledTexture.width()) / 2;
                int y = imageRect.y() + (imageRect.height() - scaledTexture.height()) / 2;

                painter.drawPixmap(x, y, scaledTexture);
            } else {
                // Fallback: рисуем стилизованный магазин если текстура не загрузилась
                painter.setBrush(ownerColor.lighter(150));
                painter.setPen(QPen(Qt::black, 1));

                // Здание магазина (уменьшенное)
                painter.drawRect(imageRect.x() + 5, imageRect.y() + 5, imageRect.width() - 10, imageRect.height() - 15);

                // Вывеска
                painter.setBrush(Qt::yellow);
                painter.drawRect(imageRect.x() + 10, imageRect.y() + 2, imageRect.width() - 20, 6);

                painter.setPen(Qt::black);
                QFont signFont = painter.font();
                signFont.setPointSize(5);
                painter.setFont(signFont);
                painter.drawText(QRect(imageRect.x() + 10, imageRect.y() + 2, imageRect.width() - 20, 6),
                                 Qt::AlignCenter, "SHOP");
            }

            // Рисуем компактную подложку цвета игрока
            painter.setBrush(ownerColor);
            painter.setPen(QPen(Qt::black, 1));
            painter.drawRect(footerRect);

            // Отображаем этап строительства на подложке
            if (!building->getIsCompleted()) {
                painter.setPen(Qt::white);
                QFont progressFont = painter.font();
                progressFont.setPointSize(7);
                progressFont.setBold(true);
                painter.setFont(progressFont);

                QString progressText = QString("%1/%2").arg(building->getMonthsBuilt()).arg(building->getBuildTime());
                painter.drawText(progressRect, Qt::AlignCenter, progressText);
            } else {
                painter.setPen(Qt::white);
                QFont progressFont = painter.font();
                progressFont.setPointSize(8);
                progressFont.setBold(true);
                painter.setFont(progressFont);
                painter.drawText(progressRect, Qt::AlignCenter, "✔");
            }
        }
    }

    // Отображаем номер клетки в левом верхнем углу
    painter.setPen(Qt::darkGray);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    painter.drawText(rect().adjusted(3, 3, -3, -3), Qt::AlignLeft | Qt::AlignTop, QString::number(cellIndex));
}

void CellWidget::mousePressEvent(QMouseEvent* event)
{
    emit cellClicked(cellIndex);
}
