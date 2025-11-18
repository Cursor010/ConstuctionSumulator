/********************************************************************************
** Form generated from reading UI file 'gamewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWINDOW_H
#define UI_GAMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWindow
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *leftVerticalLayout;
    QLabel *infoLabel;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QDoubleSpinBox *housingAdSpinBox;
    QLabel *label_2;
    QDoubleSpinBox *marketAdSpinBox;
    QPushButton *setAdvertisingButton;
    QHBoxLayout *buttonsHorizontalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buildConcreteHouseButton;
    QPushButton *buildWoodHouseButton;
    QPushButton *buildBrickHouseButton;
    QPushButton *buildMarketButton;
    QPushButton *backButton;
    QPushButton *skipTurnButton;
    QSpacerItem *horizontalSpacer;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QLabel *playersInfoLabel;

    void setupUi(QWidget *GameWindow)
    {
        if (GameWindow->objectName().isEmpty())
            GameWindow->setObjectName("GameWindow");
        GameWindow->resize(1200, 800);
        GameWindow->setStyleSheet(QString::fromUtf8("QWidget#GameWindow {\n"
"    border-image: url(D:/Prak/grass.jpg);\n"
"    }\n"
"   "));
        gridLayout_2 = new QGridLayout(GameWindow);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        leftVerticalLayout = new QVBoxLayout();
        leftVerticalLayout->setObjectName("leftVerticalLayout");
        infoLabel = new QLabel(GameWindow);
        infoLabel->setObjectName("infoLabel");
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        infoLabel->setFont(font);
        infoLabel->setAlignment(Qt::AlignLeading);

        leftVerticalLayout->addWidget(infoLabel);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        leftVerticalLayout->addItem(verticalSpacer_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);

        leftVerticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        leftVerticalLayout->addItem(verticalSpacer);

        groupBox = new QGroupBox(GameWindow);
        groupBox->setObjectName("groupBox");
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout_3->addWidget(label);

        housingAdSpinBox = new QDoubleSpinBox(groupBox);
        housingAdSpinBox->setObjectName("housingAdSpinBox");
        housingAdSpinBox->setMinimum(0.000000000000000);
        housingAdSpinBox->setMaximum(1000.000000000000000);
        housingAdSpinBox->setSingleStep(10.000000000000000);

        horizontalLayout_3->addWidget(housingAdSpinBox);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        horizontalLayout_3->addWidget(label_2);

        marketAdSpinBox = new QDoubleSpinBox(groupBox);
        marketAdSpinBox->setObjectName("marketAdSpinBox");
        marketAdSpinBox->setMinimum(0.000000000000000);
        marketAdSpinBox->setMaximum(1000.000000000000000);
        marketAdSpinBox->setSingleStep(10.000000000000000);

        horizontalLayout_3->addWidget(marketAdSpinBox);

        setAdvertisingButton = new QPushButton(groupBox);
        setAdvertisingButton->setObjectName("setAdvertisingButton");
        setAdvertisingButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #FFC107, stop: 1 #FFA000);\n"
"    border: 2px solid #FF6F00;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 10px;\n"
"    padding: 8px 12px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #FFD54F, stop: 1 #FFB300);\n"
"    border: 2px solid #FF8F00;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #FFA000, stop: 1 #FF6F00);\n"
"    padding-top: 9px;\n"
"    padding-bottom: 7px;\n"
"    }"));

        horizontalLayout_3->addWidget(setAdvertisingButton);


        leftVerticalLayout->addWidget(groupBox);

        buttonsHorizontalLayout = new QHBoxLayout();
        buttonsHorizontalLayout->setObjectName("buttonsHorizontalLayout");

        leftVerticalLayout->addLayout(buttonsHorizontalLayout);


        verticalLayout->addLayout(leftVerticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        buildConcreteHouseButton = new QPushButton(GameWindow);
        buildConcreteHouseButton->setObjectName("buildConcreteHouseButton");
        buildConcreteHouseButton->setMinimumSize(QSize(100, 50));
        buildConcreteHouseButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #2196F3, stop: 1 #1976D2);\n"
"    border: 2px solid #0D47A1;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 10px;\n"
"    padding: 8px 12px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #42A5F5, stop: 1 #1E88E5);\n"
"    border: 2px solid #1565C0;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #1976D2, stop: 1 #0D47A1);\n"
"    padding-top: 9px;\n"
"    padding-bottom: 7px;\n"
"    }"));

        horizontalLayout->addWidget(buildConcreteHouseButton);

        buildWoodHouseButton = new QPushButton(GameWindow);
        buildWoodHouseButton->setObjectName("buildWoodHouseButton");
        buildWoodHouseButton->setMinimumSize(QSize(100, 50));
        buildWoodHouseButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #4CAF50, stop: 1 #388E3C);\n"
"    border: 2px solid #1B5E20;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 10px;\n"
"    padding: 8px 12px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #66BB6A, stop: 1 #43A047);\n"
"    border: 2px solid #2E7D32;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #388E3C, stop: 1 #1B5E20);\n"
"    padding-top: 9px;\n"
"    padding-bottom: 7px;\n"
"    }"));

        horizontalLayout->addWidget(buildWoodHouseButton);

        buildBrickHouseButton = new QPushButton(GameWindow);
        buildBrickHouseButton->setObjectName("buildBrickHouseButton");
        buildBrickHouseButton->setMinimumSize(QSize(100, 50));
        buildBrickHouseButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #FF5722, stop: 1 #D84315);\n"
"    border: 2px solid #BF360C;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 10px;\n"
"    padding: 8px 12px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #FF8A65, stop: 1 #FF7043);\n"
"    border: 2px solid #E64A19;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #D84315, stop: 1 #BF360C);\n"
"    padding-top: 9px;\n"
"    padding-bottom: 7px;\n"
"    }"));

        horizontalLayout->addWidget(buildBrickHouseButton);

        buildMarketButton = new QPushButton(GameWindow);
        buildMarketButton->setObjectName("buildMarketButton");
        buildMarketButton->setMinimumSize(QSize(120, 50));
        buildMarketButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #FF9800, stop: 1 #F57C00);\n"
"    border: 2px solid #E65100;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 10px;\n"
"    padding: 8px 12px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #FFB74D, stop: 1 #FF9800);\n"
"    border: 2px solid #EF6C00;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #F57C00, stop: 1 #E65100);\n"
"    padding-top: 9px;\n"
"    padding-bottom: 7px;\n"
"    }"));

        horizontalLayout->addWidget(buildMarketButton);

        backButton = new QPushButton(GameWindow);
        backButton->setObjectName("backButton");
        backButton->setMinimumSize(QSize(120, 50));
        backButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #f44336, stop: 1 #D32F2F);\n"
"    border: 2px solid #B71C1C;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 10px;\n"
"    padding: 8px 12px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #EF5350, stop: 1 #E53935);\n"
"    border: 2px solid #C62828;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #D32F2F, stop: 1 #B71C1C);\n"
"    padding-top: 9px;\n"
"    padding-bottom: 7px;\n"
"    }"));

        horizontalLayout->addWidget(backButton);

        skipTurnButton = new QPushButton(GameWindow);
        skipTurnButton->setObjectName("skipTurnButton");
        skipTurnButton->setMinimumSize(QSize(120, 50));
        skipTurnButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #9C27B0, stop: 1 #7B1FA2);\n"
"    border: 2px solid #4A148C;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 10px;\n"
"    padding: 8px 12px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #BA68C8, stop: 1 #8E24AA);\n"
"    border: 2px solid #6A1B9A;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"    stop: 0 #7B1FA2, stop: 1 #4A148C);\n"
"    padding-top: 9px;\n"
"    padding-bottom: 7px;\n"
"    }"));

        horizontalLayout->addWidget(skipTurnButton);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        scrollArea = new QScrollArea(GameWindow);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setMinimumSize(QSize(350, 600));
        scrollArea->setMaximumSize(QSize(400, 16777215));
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 348, 786));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName("verticalLayout_2");
        playersInfoLabel = new QLabel(scrollAreaWidgetContents);
        playersInfoLabel->setObjectName("playersInfoLabel");
        QFont font1;
        font1.setPointSize(10);
        playersInfoLabel->setFont(font1);
        playersInfoLabel->setAlignment(Qt::AlignTop);
        playersInfoLabel->setWordWrap(true);

        verticalLayout_2->addWidget(playersInfoLabel);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_2->addWidget(scrollArea, 0, 2, 1, 1);


        retranslateUi(GameWindow);

        QMetaObject::connectSlotsByName(GameWindow);
    } // setupUi

    void retranslateUi(QWidget *GameWindow)
    {
        GameWindow->setWindowTitle(QCoreApplication::translate("GameWindow", "\320\230\320\263\321\200\320\276\320\262\320\276\320\265 \320\277\320\276\320\273\320\265", nullptr));
        infoLabel->setText(QCoreApplication::translate("GameWindow", "\320\245\320\276\320\264: | \320\234\320\265\321\201\321\217\321\206: 0/0 | \320\224\320\265\320\275\321\214\320\263\320\270: 0 \321\203.\320\265.", nullptr));
        groupBox->setTitle(QCoreApplication::translate("GameWindow", "\320\240\320\265\320\272\320\273\320\260\320\274\320\275\321\213\320\265 \320\261\321\216\320\264\320\266\320\265\321\202\321\213 (\321\202\321\213\321\201. \321\203.\320\265.)", nullptr));
        label->setText(QCoreApplication::translate("GameWindow", "\320\240\320\265\320\272\320\273\320\260\320\274\320\260 \320\266\320\270\320\273\321\214\321\217:", nullptr));
        label_2->setText(QCoreApplication::translate("GameWindow", "\320\240\320\265\320\272\320\273\320\260\320\274\320\260 \320\274\320\260\320\263\320\260\320\267\320\270\320\275\320\276\320\262:", nullptr));
        setAdvertisingButton->setText(QCoreApplication::translate("GameWindow", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214 \321\200\320\265\320\272\320\273\320\260\320\274\321\203", nullptr));
        buildConcreteHouseButton->setText(QCoreApplication::translate("GameWindow", "\320\221\320\265\321\202\320\276\320\275\320\275\321\213\320\271 \320\264\320\276\320\274", nullptr));
        buildWoodHouseButton->setText(QCoreApplication::translate("GameWindow", "\320\224\320\265\321\200\320\265\320\262\321\217\320\275\320\275\321\213\320\271 \320\264\320\276\320\274", nullptr));
        buildBrickHouseButton->setText(QCoreApplication::translate("GameWindow", "\320\232\320\270\321\200\320\277\320\270\321\207\320\275\321\213\320\271 \320\264\320\276\320\274", nullptr));
        buildMarketButton->setText(QCoreApplication::translate("GameWindow", "\320\234\320\260\320\263\320\260\320\267\320\270\320\275", nullptr));
        backButton->setText(QCoreApplication::translate("GameWindow", "\320\222 \320\263\320\273\320\260\320\262\320\275\320\276\320\265 \320\274\320\265\320\275\321\216", nullptr));
        skipTurnButton->setText(QCoreApplication::translate("GameWindow", "\320\237\321\200\320\276\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \321\205\320\276\320\264", nullptr));
        playersInfoLabel->setText(QCoreApplication::translate("GameWindow", "\320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265 \320\270\320\263\321\200\320\276\320\272\320\276\320\262:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameWindow: public Ui_GameWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWINDOW_H
