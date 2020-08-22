#-------------------------------------------------
#
# Project created by QtCreator 2020-08-13T15:37:39
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Robot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    secret.cpp \
    forget.cpp \
    admin.cpp \
    menu.cpp \
    pictures.cpp \
    chat.cpp \
    music.cpp \
    video.cpp \
    wallpaper.cpp

HEADERS += \
        mainwindow.h \
    secret.h \
    forget.h \
    admin.h \
    menu.h \
    pictures.h \
    chat.h \
    music.h \
    video.h \
    wallpaper.h

FORMS += \
        mainwindow.ui \
    secret.ui \
    forget.ui \
    admin.ui \
    menu.ui \
    pictures.ui \
    chat.ui \
    music.ui \
    video.ui \
    wallpaper.ui

RESOURCES += \
    image.qrc
