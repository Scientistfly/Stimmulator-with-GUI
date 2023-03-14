#-------------------------------------------------
#
# Project created by QtCreator 2014-05-02T15:47:24
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = Estimulador2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    recepcion.cpp \
    qcustomplot.cpp \
    showlivereception.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    recepcion.h \
    qcustomplot.h \
    showlivereception.h

FORMS    += mainwindow.ui \
    dialog.ui \
    recepcion.ui \
    showlivereception.ui

QMAKE_CXXFLAGS += -std=gnu++11


