#include "LH_MonitoringSource_ATITrayTools.h"
#include "LH_MonitoringTypes_ATITrayTools.h"

LH_MonitoringSource_ATITrayTools::LH_MonitoringSource_ATITrayTools(LH_QtObject *parent): LH_MonitoringSource(parent, "ATI Tray Tools") {}

bool LH_MonitoringSource_ATITrayTools::doUpdate()
{
#ifndef Q_OS_WIN
    return false;
#else
    bool resultVal = false;
    const char* mapname  = "ATITRAY_SMEM";

    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    setDataAvailable(filemap != NULL);
    if(dataAvailable())
    {
        tagTATTData* memoryData = (tagTATTData*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(tagTATTData));

        if (memoryData) {

            QString Current3DAPI = "";
            switch(memoryData->Cur3DApi)
            {
            case -1:
                Current3DAPI = "None"; break;
            case 0:
                Current3DAPI = "OpenGL"; break;
            case 1:
                Current3DAPI = "Direct3D8"; break;
            case 2:
                Current3DAPI = "Direct3D9"; break;
            case 3:
                Current3DAPI = "DirectDraw"; break;
            case 4:
                Current3DAPI = "Direct3Dx (old direct3d api)"; break;
            case 5:
                Current3DAPI = "Video Overlay"; break;
            default:
                Current3DAPI = QString("Unknown API (%1)").arg(memoryData->Cur3DApi);
            }

            updateValue("Speeds", "GPU", "GPU", (int)memoryData->CurGPU, SensorDefinition("MHz") );
            updateValue("Speeds", "MEM", "MEM", (int)memoryData->CurMEM, SensorDefinition("MHz") );

            updateValue("Temperatures", "GPU", "Current", (int)memoryData->GPUTemp, SensorDefinition("\176C") );
            updateValue("Temperatures", "GPU", "Maximum", (int)memoryData->MAXGpuTemp, SensorDefinition("\176C") );
            updateValue("Temperatures", "GPU", "Minimum", (int)memoryData->MINGpuTemp, SensorDefinition("\176C") );
            updateValue("Temperatures", "ENV", "Current", (int)memoryData->ENVTemp, SensorDefinition("\176C") );
            updateValue("Temperatures", "ENV", "Maximum", (int)memoryData->MAXEnvTemp, SensorDefinition("\176C") );
            updateValue("Temperatures", "ENV", "Minimum", (int)memoryData->MINEnvTemp, SensorDefinition("\176C") );

            updateValue("FAN Duty", "", "", (int)memoryData->FanDuty, SensorDefinition("%") );

            updateValue("FPS", "", "", (int)memoryData->CurFPS, SensorDefinition("FPS") );

            updateValue("Memory", "Used Memory", "Used Memory", (int)memoryData->MemUsed, SensorDefinition("MB") );
            updateValue("Memory", "Free Memory", "Free Video Memory", (int)memoryData->FreeVideo, SensorDefinition("MB") );
            updateValue("Memory", "Free Memory", "Free Texture Memory", (int)memoryData->FreeTexture, SensorDefinition("MB") );

            updateValue("ATI Options", "Game Profile Active", "Game Profile Active", (memoryData->isGameActive==1? "Yes" : "No") );
            updateValue("ATI Options", "Temp Mon Supported", "Temp Mon Supported", (memoryData->isTempMonSupported==1? "Yes" : "No") );

            updateValue("Graphics API", "2D/3D Mode", "2D/3D Mode", (memoryData->is3DActive==1? "3D" : "2D") );
            updateValue("Graphics API", "Currently Active API", "Currently Active API", Current3DAPI );
            updateValue("Graphics API", "Direct3D", "Antialiasing", (int)memoryData->CurD3DAA );
            updateValue("Graphics API", "Direct3D", "Anisotropy", (int)memoryData->CurD3DAF );
            updateValue("Graphics API", "OpenGL", "Antialiasing", (int)memoryData->CurOGLAA );
            updateValue("Graphics API", "OpenGL", "Anisotropy", (int)memoryData->CurOGLAF );



            UnmapViewOfFile(memoryData);
        }
        CloseHandle(filemap);
    }

    return resultVal;
#endif
}
