#-------------------------------------------------
#
# Project created by QtCreator 2014-03-30T13:39:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TEMPLATE = app
TEMPLATE = lib
CONFIG+= staticlib

TARGET = semmVis
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    playboardview.cpp \
    playboardscene.cpp

HEADERS  += mainwindow.h \
    playboardview.h \
    playboardscene.h

#LIBS += boost_system

FORMS    += mainwindow.ui
