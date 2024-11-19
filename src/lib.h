#ifndef LIB_H
#define LIB_H

/* X+ is right, Y+ is top and Z+ is forward. */

#include "krp/krp-kart.h"
#include "krp/krp-draw.h"
#include "krp/krp-track.h"
#include "krp/krp-race.h"
#include "krp/krp-replay.h"

using namespace krp::kart;
using namespace krp::draw;
using namespace krp::track;
using namespace krp::race;
using namespace krp::replay;

extern "C"
{
    __declspec(dllexport) char *GetModID();
    __declspec(dllexport) int GetModDataVersion();
    __declspec(dllexport) int GetInterfaceVersion();

    /* called when software is started */
    __declspec(dllexport) int Startup(char *savePath);

    /* called when software is closed */
    __declspec(dllexport) void Shutdown();

    /* called when event is initialized. This function is optional */
    __declspec(dllexport) void EventInit(SPluginsKartEvent_t *_pData, int _iDataSize);

    /* called when event is closed. This function is optional */
    __declspec(dllexport) void EventDeinit();

    /* called when kart goes to track. This function is optional */
    __declspec(dllexport) void RunInit(SPluginsKartSession_t *_pData, int _iDataSize);

    /* called when kart leaves the track. This function is optional */
    __declspec(dllexport) void RunDeinit();

    /* called when simulation is started / resumed. This function is optional */
    __declspec(dllexport) void RunStart();

    /* called when simulation is paused. This function is optional */
    __declspec(dllexport) void RunStop();

    /* called when a new lap is recorded. This function is optional */
    __declspec(dllexport) void RunLap(SPluginsKartLap_t *_pData, int _iDataSize);

    /* called when a split is crossed. This function is optional */
    __declspec(dllexport) void RunSplit(SPluginsKartSplit_t *_pData, int _iDataSize);

    /* _fTime is the ontrack time, in seconds. _fPos is the position on centerline, from 0 to 1. This function is optional */
    __declspec(dllexport) void RunTelemetry(SPluginsKartData_t *_pData, int _iDataSize, float _fTime, float _fPos);

    /*
    called when software is started.
    Set _piNumSprites to the number of zero-separated filenames in _pszSpriteName.
    Set _piNumFonts to the number of zero-separated filenames in _pszFontName.
    The base path for the sprite and font files is the plugins folder.
    This function is optional
    */
    __declspec(dllexport) int DrawInit(int *_piNumSprites, char **_pszSpriteName, int *_piNumFonts, char **_pszFontName);

    /*
    _iState: 0 = on track; 1 = spectate; 2 = replay.
    Set _piNumQuads to the number of quads to draw.
    Set _ppQuad to an array of SPluginQuad_t structures.
    Set _piNumString to the number of strings to draw.
    Set _ppString to an array of SPluginString_t structures.
    This function is optional
    */
    __declspec(dllexport) void Draw(int _iState, int *_piNumQuads, SPluginQuad_t **_ppQuad, int *_piNumString, SPluginString_t **_ppString);

    /*
    _pRaceData is a pointer to a float array with the longitudinal position of the start / finish line, splits and speedtrap.
    This function is optional
    */
    __declspec(dllexport) void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t *_pasSegment, float *_pfRaceData);

    /* called when event is initialized or a replay is loaded. This function is optional */
    __declspec(dllexport) void RaceEvent(SPluginsRaceEvent_t *_pData, int _iDataSize);

    /* called when event is closed. This function is optional */
    __declspec(dllexport) void RaceDeinit();

    /* This function is optional */
    __declspec(dllexport) void RaceAddEntry(SPluginsRaceAddEntry_t *_pData, int _iDataSize);

    /* This function is optional */
    __declspec(dllexport) void RaceRemoveEntry(SPluginsRaceRemoveEntry_t *_pData, int _iDataSize);

    /* This function is optional */
    __declspec(dllexport) void RaceSession(SPluginsRaceSession_t *_pData, int _iDataSize);

    /* This function is optional */
    __declspec(dllexport) void RaceSessionState(SPluginsRaceSessionState_t *_pData, int _iDataSize);

    /* This function is optional */
    __declspec(dllexport) void RaceLap(SPluginsRaceLap_t *_pData, int _iDataSize);

    /* This function is optional */
    __declspec(dllexport) void RaceSplit(SPluginsRaceSplit_t *_pData, int _iDataSize);

    /* This function is optional */
    __declspec(dllexport) void RaceSpeed(SPluginsRaceSpeed_t *_pData, int _iDataSize);

    /* This function is optional */
    __declspec(dllexport) void RaceCommunication(SPluginsRaceCommunication_t *_pData, int _iDataSize);

    /* The number of elements of _pArray if given by m_iNumEntries in _pData. This function is optional */
    __declspec(dllexport) void RaceClassification(SPluginsRaceClassification_t *_pData, int _iDataSize, SPluginsRaceClassificationEntry_t *_pArray, int _iElemSize);

    /* This function is optional */
    __declspec(dllexport) void RaceTrackPosition(int _iNumVehicles, SPluginsRaceTrackPosition_t *_pArray, int _iElemSize);

    /* This function is optional */
    __declspec(dllexport) void RaceVehicleData(SPluginsRaceVehicleData_t *_pData, int _iDataSize);

    /* Return 1 if _piSelect is set, from 0 to _iNumVehicles - 1 */
    __declspec(dllexport) int SpectateVehicles(int _iNumVehicles, SPluginsSpectateVehicle_t *_pVehicleData, int _iCurSelection, int *_piSelect);

    /* Return 1 if _piSelect is set, from 0 to _iNumCameras - 1 */
    __declspec(dllexport) int SpectateCameras(int _iNumCameras, char *_pCameraData, int _iCurSelection, int *_piSelect);
}

#endif