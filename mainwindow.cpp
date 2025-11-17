#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Устанавливаем фиксированный размер окна
    this->setFixedSize(400, 300);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startGameButton_clicked()
{
    // Создаем и показываем окно настройки
    SetupWindow *setupWindow = new SetupWindow();
    setupWindow->show();
    this->hide();

    // При закрытии окна настройки показываем главное окно снова
    connect(setupWindow, &SetupWindow::windowClosed, this, &MainWindow::show);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}
