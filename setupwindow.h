#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class SetupWindow;
}

class MainWindow;

class SetupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SetupWindow(MainWindow* mainWindow, QWidget *parent = nullptr);
    ~SetupWindow();

signals:
    void startGameSignal(const QStringList& playerNames, int totalMonths);

private slots:
    void on_playerCountSpin_valueChanged(int value);
    void on_startButton_clicked();
    void saveCurrentNames(); // Добавляем объявление

private:
    void updateNameFields();

    Ui::SetupWindow *ui;
    MainWindow* mainWindow;
    QStringList savedNames;
};

#endif
