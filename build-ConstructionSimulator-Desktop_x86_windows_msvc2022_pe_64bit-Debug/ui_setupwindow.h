/********************************************************************************
** Form generated from reading UI file 'setupwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPWINDOW_H
#define UI_SETUPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QHBoxLayout *playersLayout;
    QLabel *label_2;
    QSpinBox *playerCountSpin;
    QHBoxLayout *monthsLayout;
    QLabel *label_3;
    QSpinBox *monthsSpin;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *nameLayout;
    QHBoxLayout *player1Layout;
    QLabel *player1Label;
    QLineEdit *player1Edit;
    QHBoxLayout *player2Layout;
    QLabel *player2Label;
    QLineEdit *player2Edit;
    QHBoxLayout *player3Layout;
    QLabel *player3Label;
    QLineEdit *player3Edit;
    QHBoxLayout *player4Layout;
    QLabel *player4Label;
    QLineEdit *player4Edit;
    QHBoxLayout *player5Layout;
    QLabel *player5Label;
    QLineEdit *player5Edit;
    QPushButton *startButton;

    void setupUi(QWidget *SetupWindow)
    {
        if (SetupWindow->objectName().isEmpty())
            SetupWindow->setObjectName("SetupWindow");
        SetupWindow->resize(450, 400);
        verticalLayout = new QVBoxLayout(SetupWindow);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(SetupWindow);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        playersLayout = new QHBoxLayout();
        playersLayout->setObjectName("playersLayout");
        label_2 = new QLabel(SetupWindow);
        label_2->setObjectName("label_2");

        playersLayout->addWidget(label_2);

        playerCountSpin = new QSpinBox(SetupWindow);
        playerCountSpin->setObjectName("playerCountSpin");
        playerCountSpin->setMinimum(2);
        playerCountSpin->setMaximum(5);

        playersLayout->addWidget(playerCountSpin);


        verticalLayout->addLayout(playersLayout);

        monthsLayout = new QHBoxLayout();
        monthsLayout->setObjectName("monthsLayout");
        label_3 = new QLabel(SetupWindow);
        label_3->setObjectName("label_3");

        monthsLayout->addWidget(label_3);

        monthsSpin = new QSpinBox(SetupWindow);
        monthsSpin->setObjectName("monthsSpin");
        monthsSpin->setMinimum(6);
        monthsSpin->setMaximum(24);
        monthsSpin->setValue(12);

        monthsLayout->addWidget(monthsSpin);


        verticalLayout->addLayout(monthsLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        nameLayout = new QVBoxLayout();
        nameLayout->setObjectName("nameLayout");
        player1Layout = new QHBoxLayout();
        player1Layout->setObjectName("player1Layout");
        player1Label = new QLabel(SetupWindow);
        player1Label->setObjectName("player1Label");

        player1Layout->addWidget(player1Label);

        player1Edit = new QLineEdit(SetupWindow);
        player1Edit->setObjectName("player1Edit");

        player1Layout->addWidget(player1Edit);


        nameLayout->addLayout(player1Layout);

        player2Layout = new QHBoxLayout();
        player2Layout->setObjectName("player2Layout");
        player2Label = new QLabel(SetupWindow);
        player2Label->setObjectName("player2Label");

        player2Layout->addWidget(player2Label);

        player2Edit = new QLineEdit(SetupWindow);
        player2Edit->setObjectName("player2Edit");

        player2Layout->addWidget(player2Edit);


        nameLayout->addLayout(player2Layout);

        player3Layout = new QHBoxLayout();
        player3Layout->setObjectName("player3Layout");
        player3Label = new QLabel(SetupWindow);
        player3Label->setObjectName("player3Label");

        player3Layout->addWidget(player3Label);

        player3Edit = new QLineEdit(SetupWindow);
        player3Edit->setObjectName("player3Edit");

        player3Layout->addWidget(player3Edit);


        nameLayout->addLayout(player3Layout);

        player4Layout = new QHBoxLayout();
        player4Layout->setObjectName("player4Layout");
        player4Label = new QLabel(SetupWindow);
        player4Label->setObjectName("player4Label");

        player4Layout->addWidget(player4Label);

        player4Edit = new QLineEdit(SetupWindow);
        player4Edit->setObjectName("player4Edit");

        player4Layout->addWidget(player4Edit);


        nameLayout->addLayout(player4Layout);

        player5Layout = new QHBoxLayout();
        player5Layout->setObjectName("player5Layout");
        player5Label = new QLabel(SetupWindow);
        player5Label->setObjectName("player5Label");

        player5Layout->addWidget(player5Label);

        player5Edit = new QLineEdit(SetupWindow);
        player5Edit->setObjectName("player5Edit");

        player5Layout->addWidget(player5Edit);


        nameLayout->addLayout(player5Layout);


        verticalLayout->addLayout(nameLayout);

        startButton = new QPushButton(SetupWindow);
        startButton->setObjectName("startButton");

        verticalLayout->addWidget(startButton);


        retranslateUi(SetupWindow);

        QMetaObject::connectSlotsByName(SetupWindow);
    } // setupUi

    void retranslateUi(QWidget *SetupWindow)
    {
        SetupWindow->setWindowTitle(QCoreApplication::translate("SetupWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\270\320\263\321\200\321\213", nullptr));
        titleLabel->setText(QCoreApplication::translate("SetupWindow", "\320\241\320\270\320\274\321\203\320\273\321\217\321\202\320\276\321\200 \320\270\320\275\320\262\320\265\321\201\321\202\320\270\321\206\320\270\320\271 \320\262 \321\201\321\202\321\200\320\276\320\270\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\276", nullptr));
        label_2->setText(QCoreApplication::translate("SetupWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\270\320\263\321\200\320\276\320\272\320\276\320\262 (2-5):", nullptr));
        label_3->setText(QCoreApplication::translate("SetupWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\274\320\265\321\201\321\217\321\206\320\265\320\262 (6-24):", nullptr));
        player1Label->setText(QCoreApplication::translate("SetupWindow", "\320\230\320\263\321\200\320\276\320\272 1:", nullptr));
        player1Edit->setPlaceholderText(QCoreApplication::translate("SetupWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\270\320\274\321\217 \320\270\320\263\321\200\320\276\320\272\320\260 1", nullptr));
        player2Label->setText(QCoreApplication::translate("SetupWindow", "\320\230\320\263\321\200\320\276\320\272 2:", nullptr));
        player2Edit->setPlaceholderText(QCoreApplication::translate("SetupWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\270\320\274\321\217 \320\270\320\263\321\200\320\276\320\272\320\260 2", nullptr));
        player3Label->setText(QCoreApplication::translate("SetupWindow", "\320\230\320\263\321\200\320\276\320\272 3:", nullptr));
        player3Edit->setPlaceholderText(QCoreApplication::translate("SetupWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\270\320\274\321\217 \320\270\320\263\321\200\320\276\320\272\320\260 3", nullptr));
        player4Label->setText(QCoreApplication::translate("SetupWindow", "\320\230\320\263\321\200\320\276\320\272 4:", nullptr));
        player4Edit->setPlaceholderText(QCoreApplication::translate("SetupWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\270\320\274\321\217 \320\270\320\263\321\200\320\276\320\272\320\260 4", nullptr));
        player5Label->setText(QCoreApplication::translate("SetupWindow", "\320\230\320\263\321\200\320\276\320\272 5:", nullptr));
        player5Edit->setPlaceholderText(QCoreApplication::translate("SetupWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\270\320\274\321\217 \320\270\320\263\321\200\320\276\320\272\320\260 5", nullptr));
        startButton->setText(QCoreApplication::translate("SetupWindow", "\320\235\320\260\321\207\320\260\321\202\321\214 \320\270\320\263\321\200\321\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SetupWindow: public Ui_SetupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPWINDOW_H
