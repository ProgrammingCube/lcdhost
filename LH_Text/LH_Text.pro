TARGET = LH_Text
TEMPLATE = lib
DEFINES += LH_TEXT_LIBRARY

include(../Plugins.pri)
include(../CF/CF.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_Text.cpp \
    LH_Text.cpp \
    LH_TextTime.cpp \
    LH_TextDate.cpp \
    LH_TextCPULoad.cpp \
    LH_TextFile.cpp \
    LH_TextNumber.cpp \
    LH_TextNetInbound.cpp \
    LH_TextNetOutbound.cpp \
    LH_TextMemPhysicalFree.cpp \
    LH_TextMemPhysicalTotal.cpp \
    LH_TextMemPhysicalUsed.cpp \
    LH_TextMemVirtualFree.cpp \
    LH_TextMemVirtualTotal.cpp \
    LH_TextMemVirtualUsed.cpp \
    LH_TextStatic.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_QtPlugin_Text.h \
    LH_Text.h \
    LH_TextFile.h \
    LH_TextNumber.h


# RSS moved to WebKit
# QT += network xml
#    LH_TextRSS.cpp \
#    LH_TextRSS.h \

