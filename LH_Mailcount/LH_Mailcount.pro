TARGET = LH_Mailcount
TEMPLATE = lib
DEFINES += LH_MAILCOUNT_LIBRARY

include(../Plugins.pri)

SOURCES += \
    ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_Mailcount.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtInstance.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_Mailcount.h

RESOURCES += LH_Mailcount.qrc
