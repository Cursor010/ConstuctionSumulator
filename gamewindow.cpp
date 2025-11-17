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
    totalMonths(GameConfig::TOTAL_MONTHS),
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
    QPainter painter(this);
    QPixmap background("D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/assets/textures/grass.jpg");

    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background);
    }

    QWidget::paintEvent(event);
}

QString GameWindow::getSeasonName(int month) const
{
    // 3 месяца на сезон, блять
    int seasonIndex = (month / 3) % 4;
    switch(seasonIndex) {
    case 0: return "Весна";
    case 1: return "Лето";
    case 2: return "Осень";
    case 3: return "Зима";
    default: return "Весна";
    }
}

void GameWindow::setupGame()
{
    // Убираем фиксированный размер для gridLayout, чтобы он мог растягиваться
    ui->gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

    // Устанавливаем растягивание для всех строк и столбцов
    for (int i = 0; i < 5; i++) {
        ui->gridLayout->setRowStretch(i, 1);
        ui->gridLayout->setColumnStretch(i, 1);
    }

    // Устанавливаем одинаковые промежутки между ячейками
    ui->gridLayout->setHorizontalSpacing(2);
    ui->gridLayout->setVerticalSpacing(2);

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
    QColor playerColor = currentPlayer->getColor();

    QString seasonName = getSeasonName(currentMonth);
    QString playerNameColored = QString("<span style='color: %1;'><b>%2</b></span>")
                                    .arg(playerColor.name())
                                    .arg(currentPlayer->getName());

    ui->infoLabel->setText(QString("Ход: %1 | Месяц: %2/%3 | Сезон: %4 | Деньги: <b>%5 у.е.</b>")
                               .arg(playerNameColored)
                               .arg(currentMonth + 1)
                               .arg(totalMonths)
                               .arg(seasonName)
                               .arg(currentPlayer->getMoney()));

    QString playersInfo = "<h3 style='margin: 5px;'>Статистика игроков:</h3>";

    for (Player* player : players) {
        int completedHouses = 0;
        int completedMarkets = 0;

        QList<Player::BuildingInfo> buildings = player->getAllBuildings();
        for (const Player::BuildingInfo& building : buildings) {
            if (building.isCompleted) {
                if (building.type == Building::HOUSE_CONCRETE ||
                    building.type == Building::HOUSE_WOOD ||
                    building.type == Building::HOUSE_BRICK) {
                    completedHouses++;
                } else if (building.type == Building::MARKET) {
                    completedMarkets++;
                }
            }
        }

        playersInfo += QString("<div style='margin: 2px; padding: 3px; border: 1px solid %1; font-size: small;'>"
                               "<b>%2</b> | "
                               "💰<b>%3</b> | "
                               "🏠<b>%4</b> | "
                               "🏪<b>%5</b>"
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
    showMonthlyProfit();

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    if (currentPlayerIndex == 0) {
        currentMonth++;
        for (Player* player : players) {
            player->processMonth(players); // передаем список всех игроков
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

void GameWindow::on_buildConcreteHouseButton_clicked()
{
    if (currentPlayerHasBuilt) {
        QMessageBox::information(this, "Информация", "Вы уже построили объект в этом ходу!");
        return;
    }

    if (!players[currentPlayerIndex]->canBuild(Building::HOUSE_CONCRETE)) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно денег для постройки бетонного дома!");
        return;
    }

    buildingTypeToBuild = Building::HOUSE_CONCRETE;
}

void GameWindow::on_buildWoodHouseButton_clicked()
{
    if (currentPlayerHasBuilt) {
        QMessageBox::information(this, "Информация", "Вы уже построили объект в этом ходу!");
        return;
    }

    if (!players[currentPlayerIndex]->canBuild(Building::HOUSE_WOOD)) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно денег для постройки деревянного дома!");
        return;
    }

    buildingTypeToBuild = Building::HOUSE_WOOD;
}

void GameWindow::on_buildBrickHouseButton_clicked()
{
    if (currentPlayerHasBuilt) {
        QMessageBox::information(this, "Информация", "Вы уже построили объект в этом ходу!");
        return;
    }

    if (!players[currentPlayerIndex]->canBuild(Building::HOUSE_BRICK)) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно денег для постройки кирпичного дома!");
        return;
    }

    buildingTypeToBuild = Building::HOUSE_BRICK;
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

            QString buildingName;
            int totalStages = 0;

            switch (newBuilding->getType()) {
            case Building::HOUSE_CONCRETE:
                buildingName = "бетонный дом";
                totalStages = 6;
                break;
            case Building::HOUSE_WOOD:
                buildingName = "деревянный дом";
                totalStages = 6;
                break;
            case Building::HOUSE_BRICK:
                buildingName = "кирпичный дом";
                totalStages = 6;
                break;
            case Building::MARKET:
                buildingName = "магазин";
                totalStages = 5;
                break;
            default:
                buildingName = "здание";
                totalStages = 6;
            }

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
