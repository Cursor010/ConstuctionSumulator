#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "building.h"
#include "setupwindow.h"
#include "player.h"
#include "cellwidget.h"
#include "building.h"
#include "realestateagency.h"

#include <QWidget>
#include <QVector>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>
#include <QPixmap>

class SetupWindow;
class Player;
class CellWidget;

namespace Ui {
class GameWindow;
}

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

private:
    void setupGame();
    void updateGameState();
    void nextPlayer();
    void showMonthlyProfit();
    void endGame();
    QString getSeasonName(int month) const;

    Ui::GameWindow* ui;
    SetupWindow* setupWindow;
    QVector<Player*> players;
    QVector<CellWidget*> cells;
    int totalMonths;
    int currentMonth;
    int currentPlayerIndex;
    bool currentPlayerHasBuilt;
    Building::Type buildingTypeToBuild;
};

#endif // GAMEWINDOW_H
