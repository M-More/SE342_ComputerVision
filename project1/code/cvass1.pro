#-------------------------------------------------
#
# Project created by QtCreator 2019-11-11T13:05:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvass1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    meanfilterdialog.cpp \
    mediumfilterdialog.cpp \
    gaussianfilterdialog.cpp

HEADERS  += mainwindow.h \
    meanfilterdialog.h \
    mediumfilterdialog.h \
    gaussianfilterdialog.h

FORMS    += mainwindow.ui \
    meanfilterdialog.ui \
    mediumfilterdialog.ui \
    gaussianfilterdialog.ui

INCLUDEPATH+= C:/opencvAndQt/install/include \
             C:/opencvAndQt/install/include/opencv \
             C:/opencvAndQt/install/include/opencv2
LIBS += -L C:/opencvAndQt/install/x86/mingw/lib/libopencv_*.a \
