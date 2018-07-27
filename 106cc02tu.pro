QT += core
QT -= gui
QT += network
QT += serialport

TARGET = 106cc02tu
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tu.cpp

HEADERS += \
    tu.h

