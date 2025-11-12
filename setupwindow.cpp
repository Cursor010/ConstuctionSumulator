#include "setupwindow.h"
#include "mainwindow.h"
#include "ui_setupwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

SetupWindow::SetupWindow(MainWindow* mainWindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SetupWindow)
    , mainWindow(mainWindow)
{
    ui->setupUi(this);

    // Устанавливаем начальные значения
    ui->playerCountSpin->setValue(2);
    ui->monthsSpin->setValue(12);

    // Инициализируем начальные имена
    savedNames = {"Игрок 1", "Игрок 2", "Игрок 3", "Игрок 4", "Игрок 5"};

    // Устанавливаем начальные имена в поля
    ui->player1Edit->setText(savedNames[0]);
    ui->player2Edit->setText(savedNames[1]);
    ui->player3Edit->setText(savedNames[2]);
    ui->player4Edit->setText(savedNames[3]);
    ui->player5Edit->setText(savedNames[4]);

    // Подключаем сигналы
    connect(ui->playerCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SetupWindow::on_playerCountSpin_valueChanged);
    connect(ui->startButton, &QPushButton::clicked, this, &SetupWindow::on_startButton_clicked);

    // Инициализируем видимость полей
    updateNameFields();
}

SetupWindow::~SetupWindow()
{
    delete ui;
}

void SetupWindow::on_playerCountSpin_valueChanged(int value)
{
    updateNameFields();
}

void SetupWindow::updateNameFields()
{
    int playerCount = ui->playerCountSpin->value();

    // Управляем видимостью полей ввода
    ui->player1Label->setVisible(playerCount >= 1);
    ui->player1Edit->setVisible(playerCount >= 1);

    ui->player2Label->setVisible(playerCount >= 2);
    ui->player2Edit->setVisible(playerCount >= 2);

    ui->player3Label->setVisible(playerCount >= 3);
    ui->player3Edit->setVisible(playerCount >= 3);

    ui->player4Label->setVisible(playerCount >= 4);
    ui->player4Edit->setVisible(playerCount >= 4);

    ui->player5Label->setVisible(playerCount >= 5);
    ui->player5Edit->setVisible(playerCount >= 5);

    // Сохраняем текущие имена
    saveCurrentNames();
}

void SetupWindow::saveCurrentNames()
{
    savedNames.clear();
    savedNames.append(ui->player1Edit->text());
    savedNames.append(ui->player2Edit->text());
    savedNames.append(ui->player3Edit->text());
    savedNames.append(ui->player4Edit->text());
    savedNames.append(ui->player5Edit->text());
}

void SetupWindow::on_startButton_clicked()
{
    int playerCount = ui->playerCountSpin->value();

    // Проверяем, что все видимые имена заполнены
    if (playerCount >= 1 && ui->player1Edit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните имя игрока 1.");
        return;
    }
    if (playerCount >= 2 && ui->player2Edit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните имя игрока 2.");
        return;
    }
    if (playerCount >= 3 && ui->player3Edit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните имя игрока 3.");
        return;
    }
    if (playerCount >= 4 && ui->player4Edit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните имя игрока 4.");
        return;
    }
    if (playerCount >= 5 && ui->player5Edit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните имя игрока 5.");
        return;
    }

    int totalMonths = ui->monthsSpin->value();

    // Используем сохраненные имена
    QStringList playerNames = savedNames.mid(0, playerCount);

    emit startGameSignal(playerNames, totalMonths);
}
