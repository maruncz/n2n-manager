#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T14:53:13
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = n2n-manager
TEMPLATE = app


SOURCES += main.cpp \
    systemtray.cpp \
    manager.cpp \
    params.cpp \
    logger.cpp

HEADERS  += \
    systemtray.h \
    manager.h \
    params.h \
    logger.h

FORMS    += \
    manager.ui \
    logger.ui

RESOURCES += \
    resources.qrc
