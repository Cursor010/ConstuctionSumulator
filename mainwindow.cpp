#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupwindow.h"
#include "gamewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , setupWindow(nullptr)
    , gameWindow(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Симулятор инвестиций в строительство");
}

MainWindow::~MainWindow()
{
    delete ui;
    if (setupWindow) delete setupWindow;
    if (gameWindow) delete gameWindow;
}

void MainWindow::on_startGameButton_clicked()
{
    if (!setupWindow) {
        setupWindow = new SetupWindow(this);
        connect(setupWindow, &SetupWindow::startGameSignal, this, &MainWindow::handleGameStart);
    }
    setupWindow->show();
    this->hide();
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::handleGameStart(const QStringList& playerNames, int totalMonths)
{
    if (gameWindow) {
        delete gameWindow;
    }

    gameWindow = new GameWindow(this, playerNames, totalMonths);
    // Убрали connect к несуществующему сигналу

    gameWindow->show();

    if (setupWindow) {
        setupWindow->hide();
    }
    this->hide();
}

void MainWindow::handleBackToMenu()
{
    if (gameWindow) {
        gameWindow->hide();
        delete gameWindow;
        gameWindow = nullptr;
    }
    this->show();
}
