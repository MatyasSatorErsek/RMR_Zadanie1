#-------------------------------------------------
#
# Project created by QtCreator 2018-02-11T14:35:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demoRMR
TEMPLATE = app
win32 {
LIBS += -lws2_32
LIBS += -lWinmm
}
INCLUDEPATH += ../robot
LIBS += -L../bin -lrobot


SOURCES += main.cpp\
        mainwindow.cpp\
        map_loader.cpp\
        trajectory.cpp

HEADERS  += mainwindow.h\
            map_loader.h \
            trajectory.h

FORMS    += mainwindow.ui

#RESOURCES += priestor.txt
