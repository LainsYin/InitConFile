#-------------------------------------------------
#
# Project created by QtCreator 2015-04-13T13:50:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

OBJECTS_DIR += obj
UI_DIR += forms
RCC_DIR += rcc
MOC_DIR += moc
DESTDIR += bin

TARGET = InitConFile
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    Resource.qrc

RC_FILE += YQCico.rc

OTHER_FILES += \
    YQCico.rc
