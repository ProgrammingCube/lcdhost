#
# lh_api6.pro
#
# Builds the LCDHost plugin API6 support library
#

TARGET = lh_api6
TEMPLATE = lib
CONFIG += staticlib

load($$PWD/../../lcdhost.prf)

HEADERS = \
    lh_api6.h \
    lh_loader.h \
    lh_id.h \
    lh_input.h \
    lh_event.h \
    lh_qvariant.h \
    lh_object.h \
    lh_service.h \
    lh_plugin.h \
    lh_linkable.h \
    lh_source.h \
    lh_sink.h \
    lh_setup.h \
    lh_layoutclass.h \
    lh_layoutitem.h

SOURCES = \
    lh_loader.cpp \
    lh_event.cpp \
    lh_qvariant.cpp \
    lh_object.cpp \
    lh_service.cpp \
    lh_plugin.cpp \
    lh_linkable.cpp \
    lh_source.cpp \
    lh_sink.cpp \
    lh_setup.cpp \
    lh_layoutitem.cpp \
    lh_layoutclass.cpp
