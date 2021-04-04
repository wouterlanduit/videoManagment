#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T08:23:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoManagment
TEMPLATE = app


SOURCES += main.cpp\
    settings.cpp \
        videomanagment.cpp \
    files.cpp \
    playback.cpp \
    mytablewidget.cpp \
    ratedialog.cpp \
    optionsdialog.cpp

HEADERS  += videomanagment.h \
    files.h \
    playback.h \
    options.h \
    mytablewidget.h \
    ratedialog.h \
    optionsdialog.h \
    settings.h

FORMS    += videomanagment.ui
