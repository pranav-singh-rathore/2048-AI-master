QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

TARGET = 2048_game
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    board.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    board.h \
    mainwindow.h

FORMS += \
    mainwindow.ui