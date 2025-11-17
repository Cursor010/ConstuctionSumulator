#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QVector>
#include "building.h"

class MainWindow;
class Player;
class CellWidget;

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(MainWindow* mainWindow, const QStringList& playerNames, int totalMonths, QWidget* parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void on_buildConcreteHouseButton_clicked();
    void on_buildWoodHouseButton_clicked();
    void on_buildBrickHouseButton_clicked();
    void on_buildMarketButton_clicked();
    void on_skipTurnButton_clicked();
    void on_backButton_clicked();
    void onCellClicked(int cellIndex);

private:
    void setupGame();
    void updateGameState();
    void nextPlayer();
    void showMonthlyProfit();
    void endGame();

    Ui::GameWindow* ui;
    MainWindow* mainWindow;
    QVector<Player*> players;
    QVector<CellWidget*> cells;
    int totalMonths;
    int currentMonth;
    int currentPlayerIndex;
    bool currentPlayerHasBuilt;
    Building::Type buildingTypeToBuild;
};

#endif // GAMEWINDOW_H
