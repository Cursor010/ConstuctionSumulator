#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "mainwindow.h"
#include "player.h"
#include "cellwidget.h"
#include "building.h"
#include "realestateagency.h"
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
    QPainter painter(this);
    QPixmap background("D:/Projects_C++/СonstructionSimulator/СonstructionSimulator/assets/textures/grass.jpg");

    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background);
    }

    QWidget::paintEvent(event);
}

QString GameWindow::getSeasonName(int month) const
{
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

    // Отображаем деньги в миллионах для удобства
    double moneyInMillions = currentPlayer->getMoney() / 1000000.0;

    ui->infoLabel->setText(QString("Ход: %1 | Месяц: %2/%3 | Сезон: %4 | Деньги: <b>%5 млн у.е.</b>")
                               .arg(playerNameColored)
                               .arg(currentMonth + 1)
                               .arg(totalMonths)
                               .arg(seasonName)
                               .arg(moneyInMillions, 0, 'f', 2));

    // Показываем детальную информацию по зданиям текущего игрока
    QString buildingsInfo = "<h3 style='margin: 5px;'>Мои здания:</h3>";
    QList<Player::BuildingInfo> allBuildings = currentPlayer->getAllBuildings();

    if (allBuildings.isEmpty()) {
        buildingsInfo += "<div style='margin: 2px; padding: 3px; font-size: small;'>"
                         "Нет построенных зданий</div>";
    } else {
        for (int i = 0; i < allBuildings.size(); ++i) {
            const Player::BuildingInfo& building = allBuildings[i];
            QString buildingName;
            QString status;
            QString profitText;
            QColor profitColor = Qt::black;

            switch (building.type) {
            case Building::HOUSE_CONCRETE:
                buildingName = "🏗️ Бетонный дом";
                break;
            case Building::HOUSE_WOOD:
                buildingName = "🌲 Деревянный дом";
                break;
            case Building::HOUSE_BRICK:
                buildingName = "🧱 Кирпичный дом";
                break;
            case Building::MARKET:
                buildingName = "🏪 Магазин";
                break;
            default:
                buildingName = "❓ Неизвестное здание";
            }

            if (building.isCompleted) {
                status = "✅ Завершено";
            } else {
                status = QString("🔄 Строится (%1/%2)")
                             .arg(building.buildProgress)
                             .arg(building.totalBuildTime);
            }

            // Форматируем прибыль/убыток
            double profitInThousands = building.monthlyProfit / 1000.0;
            if (building.monthlyProfit > 0) {
                profitText = QString("💰 <span style='color: green;'>+%1 тыс.у.е.</span>")
                                 .arg(profitInThousands, 0, 'f', 1);
                profitColor = Qt::darkGreen;
            } else if (building.monthlyProfit < 0) {
                profitText = QString("💸 <span style='color: red;'>%1 тыс.у.е.</span>")
                                 .arg(profitInThousands, 0, 'f', 1);
                profitColor = Qt::red;
            } else {
                profitText = "➖ 0 у.е.";
            }

            buildingsInfo += QString("<div style='margin: 2px; padding: 3px; border: 1px solid %1; font-size: small;'>"
                                     "<b>%2</b> (клетка %3)<br>"
                                     "%4 | %5"
                                     "</div>")
                                 .arg(profitColor.name())
                                 .arg(buildingName)
                                 .arg(building.cellIndex + 1)
                                 .arg(status)
                                 .arg(profitText);
        }
    }

    // Общая статистика игроков
    QString playersInfo = "<h3 style='margin: 5px;'>Статистика игроков:</h3>";

    for (int i = 0; i < players.size(); ++i) {
        Player* player = players[i];
        int completedHouses = 0;
        int completedMarkets = 0;
        double totalProfit = 0;

        QList<Player::BuildingInfo> buildings = player->getAllBuildings();
        for (int j = 0; j < buildings.size(); ++j) {
            const Player::BuildingInfo& building = buildings[j];
            totalProfit += building.monthlyProfit;
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

        QString profitText;
        if (totalProfit > 0) {
            profitText = QString("📈 <span style='color: green;'>+%1</span>")
                             .arg(totalProfit / 1000.0, 0, 'f', 1);
        } else if (totalProfit < 0) {
            profitText = QString("📉 <span style='color: red;'>%1</span>")
                             .arg(totalProfit / 1000.0, 0, 'f', 1);
        } else {
            profitText = "➖ 0";
        }

        playersInfo += QString("<div style='margin: 2px; padding: 3px; border: 1px solid %1; font-size: small;'>"
                               "<b>%2</b> | "
                               "💰<b>%3 млн</b> | "
                               "🏠<b>%4</b> | "
                               "🏪<b>%5</b> | "
                               "Прибыль: %6 тыс."
                               "</div>")
                           .arg(player->getColor().name())
                           .arg(player->getName())
                           .arg(player->getMoney() / 1000000.0, 0, 'f', 2)
                           .arg(completedHouses)
                           .arg(completedMarkets)
                           .arg(profitText);
    }

    // Объединяем информацию о зданиях и статистику игроков
    QString allInfo = buildingsInfo + playersInfo;
    ui->playersInfoLabel->setText(allInfo);

    for (int i = 0; i < cells.size(); ++i) {
        cells[i]->update();
    }
}

void GameWindow::nextPlayer()
{
    // Показываем прибыль за прошлый месяц
    showMonthlyProfit();

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    if (currentPlayerIndex == 0) {
        currentMonth++;

        // Обрабатываем продажи жилья через риэлторское агентство
        Player::Season currentSeason = players[0]->getSeason(currentMonth);
        double totalDemand = players[0]->getHousingDemand(currentSeason);
        RealEstateAgency::processHousingSales(totalDemand, players);

        // Обработка месяца для всех игроков
        for (int i = 0; i < players.size(); ++i) {
            players[i]->processMonth(players, currentMonth);
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

    // Показываем всплывающие сообщения о прибыли/убытках
    QStringList profitMessages;

    for (int i = 0; i < profits.size(); ++i) {
        const QPair<int, double>& profit = profits[i];
        int cellIndex = profit.first;
        double amount = profit.second;

        if (amount != 0) {
            cells[cellIndex]->showProfit(amount);

            // Формируем сообщение
            QString profitType = amount > 0 ? "прибыль" : "убыток";
            QString sign = amount > 0 ? "+" : "";
            double amountInThousands = amount / 1000.0;

            profitMessages.append(QString("Клетка %1: %2%3 тыс.у.е.")
                                      .arg(cellIndex + 1)
                                      .arg(sign)
                                      .arg(amountInThousands, 0, 'f', 1));
        }
    }

    currentPlayer->clearLastMonthProfits();
}

void GameWindow::endGame()
{
    Player* winner = players[0];
    for (int i = 1; i < players.size(); ++i) {
        if (players[i]->calculateTotalCapital() > winner->calculateTotalCapital()) {
            winner = players[i];
        }
    }

    QString results = "Игра окончена!\n\nРезультаты:\n";
    for (int i = 0; i < players.size(); ++i) {
        double capitalInMillions = players[i]->calculateTotalCapital() / 1000000.0;
        results += QString("%1: %2 млн у.е.\n")
                       .arg(players[i]->getName())
                       .arg(capitalInMillions, 0, 'f', 2);
    }

    double winnerCapitalInMillions = winner->calculateTotalCapital() / 1000000.0;
    results += QString("\nПобедитель: %1 с капиталом %2 млн у.е.!")
                   .arg(winner->getName())
                   .arg(winnerCapitalInMillions, 0, 'f', 2);

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
    ui->infoLabel->setText("Выберите клетку для постройки бетонного дома");
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
    ui->infoLabel->setText("Выберите клетку для постройки деревянного дома");
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
    ui->infoLabel->setText("Выберите клетку для постройки кирпичного дома");
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
    ui->infoLabel->setText("Выберите клетку для постройки магазина");
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
                totalStages = GameConfig::CONCRETE_HOUSE_BUILD_TIME;
                break;
            case Building::HOUSE_WOOD:
                buildingName = "деревянный дом";
                totalStages = GameConfig::WOOD_HOUSE_BUILD_TIME;
                break;
            case Building::HOUSE_BRICK:
                buildingName = "кирпичный дом";
                totalStages = GameConfig::BRICK_HOUSE_BUILD_TIME;
                break;
            case Building::MARKET:
                buildingName = "магазин";
                totalStages = GameConfig::MARKET_BUILD_TIME;
                break;
            default:
                buildingName = "здание";
                totalStages = 6;
            }

            QMessageBox* msgBox = new QMessageBox(this);
            msgBox->setWindowTitle("Успех");
            msgBox->setText(QString("Начато строительство %1!\nЭтап: 1/%2\nСтоимость: %3 млн у.е.\nХод переходит следующему игроку.")
                                .arg(buildingName)
                                .arg(totalStages)
                                .arg(newBuilding->getCost() / 1000000.0, 0, 'f', 2));
            msgBox->setAttribute(Qt::WA_DeleteOnClose);

            QTimer::singleShot(2000, msgBox, &QMessageBox::accept);
            QTimer::singleShot(2000, this, &GameWindow::nextPlayer);

            msgBox->exec();
        }
    } else if (buildingTypeToBuild != Building::NO_BUILDING) {
        QMessageBox::warning(this, "Ошибка", "В этой клетке уже есть здание!");
    }
}
