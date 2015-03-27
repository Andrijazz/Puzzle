#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T17:25:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Puzzle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    puzzleconfig.cpp \
    puzzlepiece.cpp \
    puzzleboard.cpp

HEADERS  += mainwindow.h \
    puzzleconfig.h \
    puzzlepiece.h \
    puzzleboard.h

FORMS    += mainwindow.ui

RESOURCES = config.qrc
