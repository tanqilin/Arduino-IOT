CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(qtcontrollerplugin)
TEMPLATE    = lib

HEADERS     = colorplugin.h ledplugin.h videoplugin.h debugplugin.h buttonplugin.h listplugin.h dircontrolplugin.h numberplugin.h infoplugin.h qtcontroller.h \
    cardplugin.h
SOURCES     = colorplugin.cpp ledplugin.cpp videoplugin.cpp debugplugin.cpp buttonplugin.cpp listplugin.cpp dircontrolplugin.cpp numberplugin.cpp infoplugin.cpp qtcontroller.cpp \
    cardplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(color.pri)
include(video.pri)
include(dircontrol.pri)
include(debug.pri)
include(number.pri)
include(led.pri)
include(list.pri)
include(button.pri)
include(info.pri)
include(card.pri)
