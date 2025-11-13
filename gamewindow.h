#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QVector>
#include "player.h"
#include "cellwidget.h"

namespace Ui {
class GameWindow;
}

class MainWindow;

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(MainWindow* mainWindow, const QStringList& playerNames, int totalMonths, QWidget* parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void on_buildHouseButton_clicked();
    void on_buildMarketButton_clicked();
    void on_skipTurnButton_clicked();
    void on_backButton_clicked();
    void onCellClicked(int cellIndex);

private:
    Ui::GameWindow *ui;
    MainWindow* mainWindow;
    QVector<Player*> players;
    QVector<CellWidget*> cells;
    int totalMonths;
    int currentMonth;
    int currentPlayerIndex;
    bool currentPlayerHasBuilt;
    Building::Type buildingTypeToBuild;

    void setupGame();
    void updateGameState();
    void nextPlayer();
    void endGame();
    void showMonthlyProfit(); // Показ прибыли в начале хода
};

#endif
