#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "mainwindow.h"
#include "player.h"
#include "cellwidget.h"
#include "building.h"
#include <QMessageBox>
#include <QTimer>
#include <QPainter>
#include <QPixmap>

GameWindow::GameWindow(MainWindow* mainWindow, const QStringList& playerNames, int totalMonths, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::GameWindow),
    mainWindow(mainWindow),
    totalMonths(totalMonths),
    currentMonth(0),
    currentPlayerIndex(0),
    currentPlayerHasBuilt(false),
    buildingTypeToBuild(Building::NO_BUILDING)
{
    ui->setupUi(this);

    QVector<QColor> playerColors = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta};

    // Создаем игроков
    for (int i = 0; i < playerNames.size(); ++i) {
        players.append(new Player(playerNames[i], i, playerColors[i]));
    }

    setupGame();
    updateGameState();
}

GameWindow::~GameWindow()
{
    delete ui;
    qDeleteAll(players);
    qDeleteAll(cells);
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    // Рисуем фон
    QPainter painter(this);
    QPixmap background("D:/Prak/grass.jpg");

    if (!background.isNull()) {
        // Масштабируем изображение под размер окна
        painter.drawPixmap(0, 0, width(), height(), background);
    }

    // Вызываем стандартную отрисовку виджетов
    QWidget::paintEvent(event);
}

void GameWindow::setupGame()
{
    // Создаем клетки и добавляем их в gridLayout из UI
    for (int i = 0; i < 25; ++i) {
        CellWidget* cell = new CellWidget(i);
        cells.append(cell);
        ui->gridLayout->addWidget(cell, i / 5, i % 5);
        connect(cell, &CellWidget::cellClicked, this, &GameWindow::onCellClicked);
    }
}

void GameWindow::updateGameState()
{
    Player* currentPlayer = players[currentPlayerIndex];

    ui->infoLabel->setText(QString("Ход: <b>%1</b> | Месяц: %2/%3 | Деньги: <b>%4 у.е.</b>")
                               .arg(currentPlayer->getName())
                               .arg(currentMonth + 1)
                               .arg(totalMonths)
                               .arg(currentPlayer->getMoney()));

    QString playersInfo = "<h3>Статистика игроков:</h3>";

    for (Player* player : players) {
        int completedHouses = 0;
        int completedMarkets = 0;

        QList<Player::BuildingInfo> buildings = player->getAllBuildings();
        for (const Player::BuildingInfo& building : buildings) {
            if (building.isCompleted) {
                if (building.type == Building::HOUSE) {
                    completedHouses++;
                } else if (building.type == Building::MARKET) {
                    completedMarkets++;
                }
            }
        }

        playersInfo += QString("<div style='margin: 5px; padding: 5px; border: 1px solid %1;'>"
                               "<b>%2</b><br>"
                               "Деньги: <b>%3 у.е.</b><br>"
                               "Готовых домов: <b>%4</b><br>"
                               "Готовых магазинов: <b>%5</b>"
                               "</div>")
                           .arg(player->getColor().name())
                           .arg(player->getName())
                           .arg(player->getMoney())
                           .arg(completedHouses)
                           .arg(completedMarkets);
    }

    ui->playersInfoLabel->setText(playersInfo);

    for (CellWidget* cell : cells) {
        cell->update();
    }
}

void GameWindow::nextPlayer()
{
    // Показываем прибыль для зданий текущего игрока перед переходом
    showMonthlyProfit();

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    if (currentPlayerIndex == 0) {
        currentMonth++;

        // Обрабатываем месяц для всех игроков
        for (Player* player : players) {
            player->processMonth();
        }

        if (currentMonth >= totalMonths) {
            endGame();
            return;
        }
    }

    currentPlayerHasBuilt = false;
    buildingTypeToBuild = Building::NO_BUILDING;
    updateGameState();
}

void GameWindow::showMonthlyProfit()
{
    Player* currentPlayer = players[currentPlayerIndex];
    QList<QPair<int, double>> profits = currentPlayer->getLastMonthProfits();

    for (const QPair<int, double>& profit : profits) {
        int cellIndex = profit.first;
        double amount = profit.second;

        if (amount != 0) {
            cells[cellIndex]->showProfit(amount);
        }
    }

    // Очищаем прибыль после показа
    currentPlayer->clearLastMonthProfits();
}

void GameWindow::endGame()
{
    Player* winner = players[0];
    for (Player* player : players) {
        if (player->calculateTotalCapital() > winner->calculateTotalCapital()) {
            winner = player;
        }
    }

    QString results = "Игра окончена!\n\nРезультаты:\n";
    for (Player* player : players) {
        results += QString("%1: %2 у.е.\n")
                       .arg(player->getName())
                       .arg(player->calculateTotalCapital());
    }

    results += QString("\nПобедитель: %1 с капиталом %2 у.е.!")
                   .arg(winner->getName())
                   .arg(winner->calculateTotalCapital());

    QMessageBox::information(this, "Конец игры", results);
    on_backButton_clicked();
}

void GameWindow::on_buildHouseButton_clicked()
{
    if (currentPlayerHasBuilt) {
        QMessageBox::information(this, "Информация", "Вы уже построили объект в этом ходу!");
        return;
    }

    if (!players[currentPlayerIndex]->canBuild(Building::HOUSE)) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно денег для постройки дома!");
        return;
    }

    buildingTypeToBuild = Building::HOUSE;
}

void GameWindow::on_buildMarketButton_clicked()
{
    if (currentPlayerHasBuilt) {
        QMessageBox::information(this, "Информация", "Вы уже построили объект в этом ходу!");
        return;
    }

    if (!players[currentPlayerIndex]->canBuild(Building::MARKET)) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно денег для постройки магазина!");
        return;
    }

    buildingTypeToBuild = Building::MARKET;
}

void GameWindow::on_skipTurnButton_clicked()
{
    nextPlayer();
}

void GameWindow::on_backButton_clicked()
{
    if (mainWindow) {
        mainWindow->show();
    }
    this->close();
}

void GameWindow::onCellClicked(int cellIndex)
{
    if (buildingTypeToBuild != Building::NO_BUILDING && !cells[cellIndex]->getBuilding()) {
        Building* newBuilding = players[currentPlayerIndex]->build(buildingTypeToBuild, cellIndex);

        if (newBuilding) {
            cells[cellIndex]->setBuilding(newBuilding);
            cells[cellIndex]->update();

            currentPlayerHasBuilt = true;
            buildingTypeToBuild = Building::NO_BUILDING;

            updateGameState();

            QString buildingName = (newBuilding->getType() == Building::HOUSE) ? "дом" : "магазин";
            int totalStages = (newBuilding->getType() == Building::HOUSE) ? 6 : 5;

            QMessageBox* msgBox = new QMessageBox(this);
            msgBox->setWindowTitle("Успех");
            msgBox->setText(QString("Начато строительство %1!\nЭтап: 1/%2\nХод переходит следующему игроку.")
                                .arg(buildingName)
                                .arg(totalStages));
            msgBox->setAttribute(Qt::WA_DeleteOnClose);

            QTimer::singleShot(1500, msgBox, &QMessageBox::accept);
            QTimer::singleShot(1500, this, &GameWindow::nextPlayer);

            msgBox->exec();
        }
    }
}
