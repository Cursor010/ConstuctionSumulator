QT += core gui widgets

TARGET = YourAppName
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Для отладки
CONFIG += debug
CONFIG -= app_bundle

# Настройки компилятора
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Указываем стандарт C++
QMAKE_CXXFLAGS += -std=c++17

# Для Windows
win32 {
    CONFIG += console
    DEFINES += QT_DEPRECATED_WARNINGS
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += Qt5Core Qt5Gui Qt5Widgets
}
