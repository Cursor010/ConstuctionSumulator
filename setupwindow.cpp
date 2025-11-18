#include "setupwindow.h"

SetupWindow::SetupWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SetupWindow)
{
    ui->setupUi(this);

    // Устанавливаем фиксированный размер окна настройки
    this->setFixedSize(500, 400);

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
    //connect(ui->backButton, &QPushButton::clicked, this, &SetupWindow::on_backButton_clicked);

    // Подключаем сигналы изменения текста для сохранения имен
    connect(ui->player1Edit, &QLineEdit::textChanged, this, &SetupWindow::saveCurrentNames);
    connect(ui->player2Edit, &QLineEdit::textChanged, this, &SetupWindow::saveCurrentNames);
    connect(ui->player3Edit, &QLineEdit::textChanged, this, &SetupWindow::saveCurrentNames);
    connect(ui->player4Edit, &QLineEdit::textChanged, this, &SetupWindow::saveCurrentNames);
    connect(ui->player5Edit, &QLineEdit::textChanged, this, &SetupWindow::saveCurrentNames);

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
}

void SetupWindow::saveCurrentNames()
{
    // Сохраняем текущие имена из ВИДИМЫХ полей
    savedNames.clear();

    if (ui->player1Edit->isVisible()) savedNames.append(ui->player1Edit->text());
    if (ui->player2Edit->isVisible()) savedNames.append(ui->player2Edit->text());
    if (ui->player3Edit->isVisible()) savedNames.append(ui->player3Edit->text());
    if (ui->player4Edit->isVisible()) savedNames.append(ui->player4Edit->text());
    if (ui->player5Edit->isVisible()) savedNames.append(ui->player5Edit->text());
}

void SetupWindow::on_startButton_clicked()
{
    // Сохраняем имена перед запуском игры
    saveCurrentNames();

    int playerCount = ui->playerCountSpin->value();

    // Проверяем, что все имена заполнены
    for (int i = 0; i < playerCount; ++i) {
        if (i < savedNames.size() && savedNames[i].isEmpty()) {
            QMessageBox::warning(this, "Ошибка",
                                 QString("Пожалуйста, заполните имя игрока %1.").arg(i + 1));
            return;
        }
    }

    // Проверяем, что количество сохраненных имен совпадает с выбранным количеством игроков
    if (savedNames.size() != playerCount) {
        return;
    }

    int totalMonths = ui->monthsSpin->value();

    // Создаем и показываем игровое окно
    GameWindow *gameWindow = new GameWindow(this, savedNames, totalMonths);
    gameWindow->show();

    // Закрываем окно настройки
    this->close();
}

void SetupWindow::on_backButton_clicked()
{
    // Просто закрываем окно настройки
    this->close();
}
