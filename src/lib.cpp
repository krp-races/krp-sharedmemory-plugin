#include "lib.h"
#include "logger/logger.h"

char *GetModID()
{
    return "krp";
}

int GetModDataVersion()
{
    return 6;
}

int GetInterfaceVersion()
{
    return 9;
}

int Startup(char *savePath)
{
    Logger::write("Startup");
    Logger::write(savePath);
    /*
    return value is requested rate
    0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
    */
    return 0;
}

void Shutdown()
{
    Logger::write("Shutdown");
}

int DrawInit(int *_piNumSprites, char **_pszSpriteName, int *_piNumFonts, char **_pszFontName)
{
    /*
    return 0 if pointers are set
    */
    return -1;
}

void Draw(int _iState, int *_piNumQuads, SPluginQuad_t **_ppQuad, int *_piNumString, SPluginString_t **_ppString)
{
    *_piNumQuads = 0;
    *_piNumString = 0;
}

int SpectateVehicles(int _iNumVehicles, SPluginsSpectateVehicle_t *_pVehicleData, int _iCurSelection, int *_piSelect)
{
    return 0;
}

int SpectateCameras(int _iNumCameras, char *_pCameraData, int _iCurSelection, int *_piSelect)
{
    return 0;
}