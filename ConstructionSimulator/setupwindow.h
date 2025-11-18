#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include "ui_setupwindow.h"
#include "gamewindow.h"
#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class SetupWindow;
}

class SetupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SetupWindow(QWidget *parent = nullptr);
    ~SetupWindow();

signals:
    void windowClosed();

private slots:
    void on_playerCountSpin_valueChanged(int value);
    void on_startButton_clicked();
    void on_backButton_clicked();
    void saveCurrentNames();

private:
    void updateNameFields();

    Ui::SetupWindow *ui;
    QStringList savedNames;
};

#endif // SETUPWINDOW_H
