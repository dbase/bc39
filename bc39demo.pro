#-------------------------------------------------
#
# Project created by QtCreator 2014-04-13T10:50:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bc39demo
TEMPLATE = app


SOURCES += main.cc\
        dialog.cc \
    bc39/bc39.cc

HEADERS  += dialog.h \
    bc39/bc39.h

FORMS    += dialog.ui
