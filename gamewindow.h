#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QVector>
#include <QStringList>
#include "building.h"

class MainWindow;
class Player;
class CellWidget;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(MainWindow* mainWindow, const QStringList& playerNames, int totalMonths, QWidget* parent = nullptr);
    ~GameWindow();

<<<<<<< HEAD
=======
signals:
    void turnCompleted(); // Сигнал о завершении хода

>>>>>>> front
private slots:
    void onBuildHouseClicked();
    void onBuildMarketClicked();
    void onSkipTurnClicked();
    void onCellClicked(int cellIndex);
    void backToMainMenu();

private:
    void setupUI();
    void updateGameState();
    void highlightAvailableCells();
    void resetCellHighlights();
    void nextPlayer();
    void endGame();

    MainWindow* mainWindow;
    QVector<Player*> players;
    QVector<CellWidget*> cells;
    int totalMonths;
    int currentMonth;
    int currentPlayerIndex;
    bool currentPlayerHasBuilt;
    Building::Type buildingTypeToBuild;

    QLabel* infoLabel;
    QLabel* playersInfoLabel;
};

#endif
