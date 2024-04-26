#-------------------------------------------------
#
# Project created by QtCreator 2018-10-14T14:42:05
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Added to provide backwards compatibility (C++11 support)
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11
}

TARGET = fau201-gui
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
    about.cpp \
    main.cpp \
    mainwindow.cpp \
    serial.cpp

HEADERS += \
    about.h \
    mainwindow.h \
    serial.h

FORMS += \
    about.ui \
    mainwindow.ui \
    serial.ui

RESOURCES += \
    resources.qrc

# Added installation option
unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    target.path = $$PREFIX/bin
    icon.files += icons/fau201-gui.png
    icon.path = $$PREFIX/share/icons/hicolor/128x128/apps
    shortcut.files = misc/fau201-gui.desktop
    shortcut.path = $$PREFIX/share/applications
    INSTALLS += icon
    INSTALLS += shortcut
}

!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/fau201-gui.png \
    misc/fau201-gui.desktop
