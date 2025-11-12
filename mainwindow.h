#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class SetupWindow;
class GameWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startGameButton_clicked();
    void on_exitButton_clicked();
    void handleGameStart(const QStringList& playerNames, int totalMonths);
    void handleBackToMenu();

private:
    Ui::MainWindow *ui;
    SetupWindow* setupWindow;
    GameWindow* gameWindow;
};

#endif
