TARGET = LH_Monitoring
TEMPLATE = lib
DEFINES += LH_Monitoring_LIBRARY
QT += xml

include(../Plugins.pri)
include(../cf.pri)

HEADERS += \
    LH_QtPlugin_Monitoring.h \
    SpeedFan/LH_SpeedFanImage.h \
    SpeedFan/LH_SpeedFanGraph.h \
    SpeedFan/LH_SpeedFanData.h \
    _Templates/LH_MonitoringText.h \
    _Templates/LH_MonitoringImage.h \
    _Templates/LH_MonitoringGraph.h \
    _Templates/LH_MonitoringData.h \
    _Templates/LH_MonitoringConditionImage.h \
    ../LH_QtPlugin.h \
    ../LH_QtSetupItem.h \
    ../LH_QtObject.h \
    ../LH_Graph/LH_Graph.h \
    ../LH_Text/LH_Text.h \
    Afterburner/LH_AfterburnerText.h \
    Afterburner/LH_AfterburnerImage.h \
    Afterburner/LH_AfterburnerGraph.h \
    Afterburner/LH_AfterburnerData.h \
    Aida64/LH_Aida64Text.h \
    Aida64/LH_Aida64Image.h \
    Aida64/LH_Aida64Graph.h \
    Aida64/LH_Aida64Data.h \
    ATITrayTools/LH_ATITrayToolsText.h \
    ATITrayTools/LH_ATITrayToolsImage.h \
    ATITrayTools/LH_ATITrayToolsGraph.h \
    ATITrayTools/LH_ATITrayToolsData.h \
    CoreTemp/LH_CoreTempText.h \
    CoreTemp/LH_CoreTempImage.h \
    CoreTemp/LH_CoreTempGraph.h \
    CoreTemp/LH_CoreTempData.h \
    Fraps/LH_FrapsText.h \
    Fraps/LH_FrapsImage.h \
    Fraps/LH_FrapsData.h \
    Fraps/LH_FrapsConditionImage.h \
    GPUZ/LH_GPUZText.h \
    GPUZ/LH_GPUZImage.h \
    GPUZ/LH_GPUZGraph.h \
    GPUZ/LH_GPUZData.h \
    RivaTuner/LH_RivaTunerText.h \
    RivaTuner/LH_RivaTunerImage.h \
    RivaTuner/LH_RivaTunerGraph.h \
    RivaTuner/LH_RivaTunerData.h \
    ../LH_Bar/LH_Bar.h \
    _Templates/LH_MonitoringBar.h \
    SpeedFan/LH_SpeedFanBar.h \
    Afterburner/LH_AfterburnerBar.h \
    Aida64/LH_Aida64Bar.h \
    CoreTemp/LH_CoreTempBar.h \
    GPUZ/LH_GPUZBar.h \
    RivaTuner/LH_RivaTunerBar.h \
    Fraps/LH_FrapsGraph.h \
    Fraps/LH_FrapsBar.h \
    ATITrayTools/LH_ATITrayToolsBar.h \
    _Templates/LH_MonitoringDial.h \
    SpeedFan/LH_SpeedFanDial.h \
    ../LH_Dial/LH_Dial.h \
    ATITrayTools/LH_ATITrayToolsDial.h \
    Afterburner/LH_AfterburnerDial.h \
    Aida64/LH_Aida64Dial.h \
    CoreTemp/LH_CoreTempDial.h \
    Fraps/LH_FrapsDial.h \
    GPUZ/LH_GPUZDial.h \
    RivaTuner/LH_RivaTunerDial.h \
    ../LH_QtInstance.h \
    LH_RivaWriter.h \
    LH_AidaWriter.h \
    SpeedFan/LH_SpeedFanText.h \
    ../lh_plugin.h \
    Logitech/LH_LogitechText.h \
    Logitech/LH_LogitechData.h \
    Logitech/LH_LogitechImage.h \
    Logitech/LH_LogitechConditionImage.h \
    Logitech/LH_LogitechBar.h \
    Logitech/LH_LogitechDial.h

SOURCES += \
    LH_QtPlugin_Monitoring.cpp \
    SpeedFan/LH_SpeedFanImage.cpp \
    SpeedFan/LH_SpeedFanGraph.cpp \
    SpeedFan/LH_SpeedFanData.cpp \
    _Templates/LH_MonitoringText.cpp \
    _Templates/LH_MonitoringImage.cpp \
    _Templates/LH_MonitoringGraph.cpp \
    _Templates/LH_MonitoringData.cpp \
    _Templates/LH_MonitoringConditionImage.cpp \
    ../LH_QtPlugin.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtObject.cpp \
    ../LH_Graph/LH_Graph.cpp \
    ../LH_Text/LH_Text.cpp \
    Afterburner/LH_AfterburnerText.cpp \
    Afterburner/LH_AfterburnerImage.cpp \
    Afterburner/LH_AfterburnerGraph.cpp \
    Afterburner/LH_AfterburnerData.cpp \
    Aida64/LH_Aida64Text.cpp \
    Aida64/LH_Aida64Image.cpp \
    Aida64/LH_Aida64Graph.cpp \
    Aida64/LH_Aida64Data.cpp \
    ATITrayTools/LH_ATITrayToolsText.cpp \
    ATITrayTools/LH_ATITrayToolsImage.cpp \
    ATITrayTools/LH_ATITrayToolsGraph.cpp \
    ATITrayTools/LH_ATITrayToolsData.cpp \
    CoreTemp/LH_CoreTempText.cpp \
    CoreTemp/LH_CoreTempImage.cpp \
    CoreTemp/LH_CoreTempGraph.cpp \
    CoreTemp/LH_CoreTempData.cpp \
    Fraps/LH_FrapsText.cpp \
    Fraps/LH_FrapsImage.cpp \
    Fraps/LH_FrapsData.cpp \
    Fraps/LH_FrapsConditionImage.cpp \
    GPUZ/LH_GPUZText.cpp \
    GPUZ/LH_GPUZImage.cpp \
    GPUZ/LH_GPUZGraph.cpp \
    GPUZ/LH_GPUZData.cpp \
    RivaTuner/LH_RivaTunerText.cpp \
    RivaTuner/LH_RivaTunerImage.cpp \
    RivaTuner/LH_RivaTunerGraph.cpp \
    RivaTuner/LH_RivaTunerData.cpp \
    ../LH_Bar/LH_Bar.cpp \
    _Templates/LH_MonitoringBar.cpp \
    SpeedFan/LH_SpeedFanBar.cpp \
    Afterburner/LH_AfterburnerBar.cpp \
    Aida64/LH_Aida64Bar.cpp \
    CoreTemp/LH_CoreTempBar.cpp \
    GPUZ/LH_GPUZBar.cpp \
    RivaTuner/LH_RivaTunerBar.cpp \
    Fraps/LH_FrapsGraph.cpp \
    Fraps/LH_FrapsBar.cpp \
    ATITrayTools/LH_ATITrayToolsBar.cpp \
    _Templates/LH_MonitoringDial.cpp \
    SpeedFan/LH_SpeedFanDial.cpp \
    ../LH_Dial/LH_Dial.cpp \
    ATITrayTools/LH_ATITrayToolsDial.cpp \
    Afterburner/LH_AfterburnerDial.cpp \
    Aida64/LH_Aida64Dial.cpp \
    CoreTemp/LH_CoreTempDial.cpp \
    Fraps/LH_FrapsDial.cpp \
    GPUZ/LH_GPUZDial.cpp \
    RivaTuner/LH_RivaTunerDial.cpp \
    ../LH_QtInstance.cpp \
    LH_RivaWriter.cpp \
    LH_AidaWriter.cpp \
    SpeedFan/LH_SpeedFanText.cpp \
    Logitech/LH_LogitechText.cpp \
    Logitech/LH_LogitechData.cpp \
    Logitech/LH_LogitechImage.cpp \
    Logitech/LH_LogitechConditionImage.cpp \
    Logitech/LH_LogitechBar.cpp \
    Logitech/LH_LogitechDial.cpp
