TARGET = LH_Graph
TEMPLATE = lib
DEFINES += LH_GRAPH_LIBRARY

include(../Plugins.pri)

SOURCES += \
    LH_QtPlugin_Graph.cpp \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtCPU.cpp \
    ../LH_QtNetwork.cpp \
    LH_Graph.cpp \
    LH_GraphCPUAverage.cpp \
    LH_GraphCPUHistogram.cpp \
    LH_GraphMemPhysical.cpp \
    LH_GraphMemVirtual.cpp \
    LH_GraphNetIn.cpp \
    LH_GraphNetOut.cpp \
    ../LH_QtInstance.cpp \
    ../LH_Qt_array_int_ui.cpp \
    ../LH_Qt_array_string_ui.cpp
	
HEADERS += \
    LH_QtPlugin_Graph.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtCPU.h \
    ../LH_QtNetwork.h \
    LH_Graph.h \
    ../LH_QtInstance.h \
    ../LH_Qt_array_double.h \
    ../LH_Qt_array_int.h \
    ../LH_Qt_array_int_ui.h \
    ../LH_Qt_array_string.h \
    ../LH_Qt_array_string_ui.h
