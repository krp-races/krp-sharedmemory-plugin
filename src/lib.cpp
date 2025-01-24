#include "lib.h"

int bestLapPos = -1;
Memory<SKartData_t> memKartData(SHARED_MEMORY_KARTDATA_NAME);
Memory<STrackData_t> memTrackData(SHARED_MEMORY_TRACKDATA_NAME);

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
    Logger::write(LogLevel::INFO, "Startup");
    Logger::write(LogLevel::INFO, savePath);

    memKartData.create();
    memTrackData.create();

    /*
    return value is requested rate
    0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
    */
    return 0;
}

void Shutdown()
{
    Logger::write(LogLevel::INFO, "Shutdown");
}

void EventInit(SPluginsKartEvent_t *_pData, int _iDataSize)
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;

    pKartData->driveType = (EDriveType)_pData->m_iDriveType;
    pKartData->numberOfGears = _pData->m_iNumberOfGears;
    pKartData->maxRPM = _pData->m_iMaxRPM;
    pKartData->limiter = _pData->m_iLimiter;
    pKartData->shiftRPM = _pData->m_iShiftRPM;
    pKartData->coolingType = (ECoolingType)_pData->m_iEngineCooling;
    pKartData->engineOptTemperature = _pData->m_fEngineOptTemperature;
    pKartData->engineTemperatureAlarm[0] = _pData->m_afEngineTemperatureAlarm[0];
    pKartData->engineTemperatureAlarm[1] = _pData->m_afEngineTemperatureAlarm[1];
    pKartData->maxFuel = _pData->m_fMaxFuel;
    pKartData->trackLength = _pData->m_fTrackLength;

    for (int i = 0; i < STRING_MAX_LENGTH; i++)
    {
        pKartData->driverName[i] = _pData->m_szDriverName[i];
        pKartData->kartID[i] = _pData->m_szKartID[i];
        pKartData->kartName[i] = _pData->m_szKartName[i];
        pKartData->category[i] = _pData->m_szCategory[i];
        pKartData->dash[i] = _pData->m_szDash[i];
        pKartData->trackId[i] = _pData->m_szTrackID[i];
        pKartData->trackName[i] = _pData->m_szTrackName[i];
    }

    pKartData->sequenceNumber++;
    memKartData.write();
}

void EventDeinit()
{
    memKartData.reset();
    memTrackData.reset();
}

void RunInit(SPluginsKartSession_t *_pData, int _iDataSize)
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;

    pKartData->driving = true;
    for (int i = 0; i < STRING_MAX_LENGTH; i++)
        pKartData->setupFileName[i] = _pData->m_szSetupFileName[i];

    pKartData->sequenceNumber++;
    memKartData.write();
}

void RunDeinit()
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;

    pKartData->driving = false;
    for (int i = 0; i < STRING_MAX_LENGTH; i++)
        pKartData->setupFileName[i] = '\0';

    pKartData->sequenceNumber++;
    memKartData.write();
}

void RunStart()
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;
    pKartData->paused = false;
    pKartData->sequenceNumber++;
    memKartData.write();
}

void RunStop()
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;
    pKartData->paused = true;
    pKartData->sequenceNumber++;
    memKartData.write();
}

void RunTelemetry(SPluginsKartData_t *_pData, int _iDataSize, float _fTime, float _fPos)
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;

    pKartData->driveTime = _fTime;
    pKartData->trackPos = _fPos;
    pKartData->rpm = _pData->m_iRPM;
    pKartData->cylinderHeadTemperature = _pData->m_fCylinderHeadTemperature;
    pKartData->waterTemperature = _pData->m_fWaterTemperature;
    pKartData->gear = _pData->m_iGear;
    pKartData->fuel = _pData->m_fFuel;
    pKartData->speedometer = _pData->m_fSpeedometer;
    pKartData->posX = _pData->m_fPosX;
    pKartData->posY = _pData->m_fPosY;
    pKartData->posZ = _pData->m_fPosZ;
    pKartData->velocityX = _pData->m_fVelocityX;
    pKartData->velocityY = _pData->m_fVelocityY;
    pKartData->velocityZ = _pData->m_fVelocityZ;
    pKartData->accelerationX = _pData->m_fAccelerationX;
    pKartData->accelerationY = _pData->m_fAccelerationY;
    pKartData->accelerationZ = _pData->m_fAccelerationZ;
    pKartData->yaw = _pData->m_fYaw;
    pKartData->pitch = _pData->m_fPitch;
    pKartData->roll = _pData->m_fRoll;
    pKartData->yawVelocity = _pData->m_fYawVelocity;
    pKartData->pitchVelocity = _pData->m_fPitchVelocity;
    pKartData->rollVelocity = _pData->m_fRollVelocity;
    pKartData->inputSteer = _pData->m_fInputSteer;
    pKartData->inputThrottle = _pData->m_fInputThrottle;
    pKartData->inputBrake = _pData->m_fInputBrake;
    pKartData->inputFrontBrakes = _pData->m_fInputFrontBrakes;
    pKartData->inputClutch = _pData->m_fInputClutch;
    pKartData->steerTorque = _pData->m_fSteerTorque;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            pKartData->rot[i][j] = _pData->m_aafRot[i][j];

    for (int i = 0; i < 4; i++)
    {
        pKartData->wheelSpeed[i] = _pData->m_afWheelSpeed[i];
        pKartData->wheelMaterial[i] = _pData->m_aiWheelMaterial[i];
    }

    pKartData->sequenceNumber++;
    memKartData.write();
}

void RunLap(SPluginsKartLap_t *_pData, int _iDataSize)
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;

    // Last Lap
    pKartData->lap = _pData->m_iLapNum;
    pKartData->lastLapTime = _pData->m_iLapTime;
    pKartData->lastLapValid = _pData->m_iInvalid;

    // Best Lap
    if (pKartData->lastLapValid && (_pData->m_iPos <= bestLapPos || bestLapPos == -1))
    {
        pKartData->bestLapTime = _pData->m_iLapTime;
        bestLapPos = _pData->m_iPos;
    }

    // Estimated Time
    pKartData->estimatedLapTimeToLastLap = pKartData->lastLapTime;
    pKartData->estimatedLapTimeToBestLap = pKartData->bestLapTime;

    // Reset deltas
    pKartData->lastLapDeltaToLastLap = pKartData->lapDeltaToLastLap;
    pKartData->lastLapDeltaToBestLap = pKartData->lapDeltaToBestLap;
    pKartData->lapDeltaToLastLap = 0;
    pKartData->lapDeltaToBestLap = 0;

    pKartData->sequenceNumber++;
}

void RunSplit(SPluginsKartSplit_t *_pData, int _iDataSize)
{
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;

    // Maximum of 5 splits per track.
    if (_pData->m_iSplit >= MAX_SPLITS)
        return;

    // Last Split
    int lastSplit = pKartData->lastSplits[_pData->m_iSplit];
    pKartData->lastSplitIndex = _pData->m_iSplit;
    pKartData->lastSplits[_pData->m_iSplit] = _pData->m_iSplitTime;

    // Best Split
    int bestSplit = pKartData->bestSplits[_pData->m_iSplit];
    if (bestSplit == 0 || _pData->m_iSplitTime < bestSplit)
        pKartData->bestSplits[_pData->m_iSplit] = _pData->m_iSplitTime;

    // Deltas
    pKartData->lapDeltaToLastLap += lastSplit - _pData->m_iSplitTime;
    pKartData->lapDeltaToBestLap += _pData->m_iBestDiff;

    // Estimated Time
    pKartData->estimatedLapTimeToLastLap += lastSplit - _pData->m_iSplitTime;
    pKartData->estimatedLapTimeToBestLap += _pData->m_iBestDiff;

    pKartData->sequenceNumber++;
    memKartData.write();
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
    SKartData_t *pKartData = memKartData.get();
    pKartData->sequenceNumber++;
    pKartData->state = (EGameState)_iState;
    pKartData->sequenceNumber++;
    memKartData.write();

    *_piNumQuads = 0;
    *_piNumString = 0;
}

void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t *_pasSegment, float *_pfRaceData)
{
    STrackData_t *pTrackData = memTrackData.get();
    pTrackData->sequenceNumber++;

    pTrackData->numSegments = _iNumSegments;

    for (int i = 0; i < _iNumSegments; i++)
    {
        STrackSegment_t *pSegment = &pTrackData->segments[i];
        pSegment->type = (ETrackSegmentType)_pasSegment[i].m_iType;
        pSegment->length = _pasSegment[i].m_fLength;
        pSegment->radius = _pasSegment[i].m_fRadius;
        pSegment->angle = _pasSegment[i].m_fAngle;
        pSegment->startX = _pasSegment[i].m_afStart[0];
        pSegment->startY = _pasSegment[i].m_afStart[1];
        pSegment->startZ = _pasSegment[i].m_fHeight;
    }

    pTrackData->sequenceNumber++;
    memTrackData.write();
}

int SpectateVehicles(int _iNumVehicles, SPluginsSpectateVehicle_t *_pVehicleData, int _iCurSelection, int *_piSelect)
{
    return 0;
}

int SpectateCameras(int _iNumCameras, char *_pCameraData, int _iCurSelection, int *_piSelect)
{
    return 0;
}