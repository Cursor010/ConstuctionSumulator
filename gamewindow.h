#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QList>
#include <QColor>
#include <QStringList>

namespace Ui {
class GameWindow;
}

class SetupWindow;
class Player;
class CellWidget;
class Building;

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(SetupWindow* setupWindow, const QStringList& playerNames, int totalMonths, QWidget* parent = nullptr);
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
    void on_setAdvertisingButton_clicked();

private:
    Ui::GameWindow* ui;
    SetupWindow* setupWindow;
    int totalMonths;
    int currentMonth;
    int currentPlayerIndex;
    bool currentPlayerHasBuilt;
    int buildingTypeToBuild;
    QList<Player*> players;
    QList<CellWidget*> cells;

    void setupGame();
    void updateGameState();
    void nextPlayer();
    void showMonthlyProfit();
    void endGame();
    QString getSeasonName(int month) const;
};

#endif // GAMEWINDOW_H
