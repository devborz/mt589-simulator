QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ICON = MT589.icns
SOURCES += \
    ListModels.cpp \
    cpe.cpp \
    emulator.cpp \
    main.cpp \
    mainwindow.cpp \
    mcu.cpp \
    ram.cpp \
    rom.cpp

HEADERS += \
    ListModels.h \
    Model.h \
    cpe.h \
    emulator.hpp \
    mainwindow.h \
    mcu.h \
    mt_global.h \
    ram.h \
    rom.h \
    util.hpp

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    MT589_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    logo.png
