#include "gamewindow.h"
#include "mainwindow.h"
#include "player.h"
#include "cellwidget.h"
#include "building.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

const int HOUSE_COST = 8000000;
const int MARKET_COST = 2500000;
const int HOUSE_BUILD_TIME = 6;
const int MARKET_BUILD_TIME = 3;
const int INITIAL_MONEY = 37000000;

GameWindow::GameWindow(MainWindow* mainWindow, const QStringList& playerNames, int totalMonths, QWidget* parent)
    : QWidget(parent), mainWindow(mainWindow), totalMonths(totalMonths), currentMonth(0),
    currentPlayerIndex(0), currentPlayerHasBuilt(false), buildingTypeToBuild(Building::NO_BUILDING)
{
    QVector<QColor> playerColors = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta};
    for (int i = 0; i < playerNames.size(); ++i) {
        players.append(new Player(playerNames[i], i, playerColors[i]));
    }

    setupUI();
    updateGameState();
}

GameWindow::~GameWindow()
{
    qDeleteAll(players);
    qDeleteAll(cells);
}

void GameWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    infoLabel = new QLabel();
    QFont infoFont = infoLabel->font();
    infoFont.setPointSize(14);
    infoLabel->setFont(infoFont);
    mainLayout->addWidget(infoLabel);

    QGridLayout* gridLayout = new QGridLayout();
    for (int i = 0; i < 25; ++i) {
        CellWidget* cell = new CellWidget(i);
        cells.append(cell);
        gridLayout->addWidget(cell, i / 5, i % 5);
        connect(cell, &CellWidget::cellClicked, this, &GameWindow::onCellClicked);
    }
    mainLayout->addLayout(gridLayout);

    QHBoxLayout* controlLayout = new QHBoxLayout();

    QPushButton* buildHouseButton = new QPushButton("Построить дом\n(8 млн)");
    QPushButton* buildMarketButton = new QPushButton("Построить магазин\n(2.5 млн)");
    QPushButton* skipTurnButton = new QPushButton("Пропустить ход");
    QPushButton* backButton = new QPushButton("В главное меню");

    connect(buildHouseButton, &QPushButton::clicked, this, &GameWindow::onBuildHouseClicked);
    connect(buildMarketButton, &QPushButton::clicked, this, &GameWindow::onBuildMarketClicked);
    connect(skipTurnButton, &QPushButton::clicked, this, &GameWindow::onSkipTurnClicked);
    connect(backButton, &QPushButton::clicked, this, &GameWindow::backToMainMenu);

    controlLayout->addWidget(buildHouseButton);
    controlLayout->addWidget(buildMarketButton);
    controlLayout->addWidget(skipTurnButton);
    controlLayout->addWidget(backButton);

    mainLayout->addLayout(controlLayout);

    playersInfoLabel = new QLabel();
    playersInfoLabel->setWordWrap(true);
    mainLayout->addWidget(playersInfoLabel);

    setLayout(mainLayout);
}

void GameWindow::updateGameState()
{
    Player* currentPlayer = players[currentPlayerIndex];

    infoLabel->setText(QString("Ход: %1 | Месяц: %2/%3 | Деньги: %4 у.е.")
                           .arg(currentPlayer->getName())
                           .arg(currentMonth + 1)
                           .arg(totalMonths)
                           .arg(currentPlayer->getMoney()));

    QString playersInfo = "Состояние игроков:\n";
    for (Player* player : players) {
        int capital = player->calculateTotalCapital();
        playersInfo += QString("%1: %2 у.е. (капитал: %3 у.е.)\n")
                           .arg(player->getName())
                           .arg(player->getMoney())
                           .arg(capital);
    }
    playersInfoLabel->setText(playersInfo);

    for (CellWidget* cell : cells) {
        cell->update();
    }
}

void GameWindow::highlightAvailableCells()
{
    for (CellWidget* cell : cells) {
        if (!cell->getBuilding()) {
            cell->setHighlighted(true);
            cell->setHighlightColor(players[currentPlayerIndex]->getColor());
            cell->update();
        }
    }
}

void GameWindow::resetCellHighlights()
{
    for (CellWidget* cell : cells) {
        cell->setHighlighted(false);
        cell->update();
    }
}

void GameWindow::nextPlayer()
{
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    if (currentPlayerIndex == 0) {
        currentMonth++;

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
    resetCellHighlights();
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
    backToMainMenu();
}

void GameWindow::backToMainMenu()
{
    mainWindow->show();
    this->close();
}

void GameWindow::onBuildHouseClicked()
{
    if (currentPlayerHasBuilt) {
        QMessageBox::information(this, "Информация", "Вы уже построили объект в этом ходу!");
        return;
    }

    if (!players[currentPlayerIndex]->canBuild(Building::HOUSE)) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно денег для постройки дома!");
        return;
    }

    resetCellHighlights();
    highlightAvailableCells();
    buildingTypeToBuild = Building::HOUSE;
}

void GameWindow::onBuildMarketClicked()
{
    if (currentPlayerHasBuilt) {
        QMessageBox::information(this, "Информация", "Вы уже построили объект в этом ходу!");
        return;
    }

    if (!players[currentPlayerIndex]->canBuild(Building::MARKET)) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно денег для постройки магазина!");
        return;
    }

    resetCellHighlights();
    highlightAvailableCells();
    buildingTypeToBuild = Building::MARKET;
}

void GameWindow::onSkipTurnClicked()
{
    nextPlayer();
}

void GameWindow::onCellClicked(int cellIndex)
{
    if (buildingTypeToBuild != Building::NO_BUILDING && !cells[cellIndex]->getBuilding()) {
        Building* newBuilding = players[currentPlayerIndex]->build(buildingTypeToBuild, cellIndex);

        if (newBuilding) {
            cells[cellIndex]->setBuilding(newBuilding);
            cells[cellIndex]->update();

            resetCellHighlights();
            currentPlayerHasBuilt = true;
            buildingTypeToBuild = Building::NO_BUILDING;

            updateGameState();

            QString buildingName = (newBuilding->getType() == Building::HOUSE) ? "дом" : "магазин";
            int totalStages = (newBuilding->getType() == Building::HOUSE) ? HOUSE_BUILD_TIME : MARKET_BUILD_TIME;
            QMessageBox::information(this, "Успех",
                                     QString("Начато строительство %1!\nЭтап: 1/%2")
                                         .arg(buildingName)
                                         .arg(totalStages));
        }
    }
}
