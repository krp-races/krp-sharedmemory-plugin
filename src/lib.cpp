#include "lib.h"
#include "memory/memory.cpp"

Memory<SSharedMemory_t> mem(SHARED_MEMORY_NAME);
EGameState prevGameState = EGameState::CLOSED;
int bestLapPos = -1;

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

    mem.create();
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = EGameState::MENU;
    memData->sequenceNumber++;
    mem.write();

    Logger::write(LogLevel::INFO, "Startup done");

    /*
    return value is requested rate
    0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
    */
    return 0;
}

void Shutdown()
{
    Logger::write(LogLevel::INFO, "Shutdown");

    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = EGameState::CLOSED;
    memData->sequenceNumber++;
    mem.write();
    mem.close();

    Logger::write(LogLevel::INFO, "Shutdown done");
}

void EventInit(SPluginsKartEvent_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = EGameState::PIT;
    memData->driveType = (EDriveType)_pData->m_iDriveType;
    memData->numberOfGears = _pData->m_iNumberOfGears;
    memData->maxRpm = _pData->m_iMaxRPM;
    memData->limiter = _pData->m_iLimiter;
    memData->shiftRpm = _pData->m_iShiftRPM;
    memData->engineCooling = (EEngineCooling)_pData->m_iEngineCooling;
    memData->engineOptTemperature = _pData->m_fEngineOptTemperature;
    memData->engineTemperatureAlarm[0] = _pData->m_afEngineTemperatureAlarm[0];
    memData->engineTemperatureAlarm[1] = _pData->m_afEngineTemperatureAlarm[1];
    memData->maxFuel = _pData->m_fMaxFuel;
    memData->trackLength = _pData->m_fTrackLength;

    for (int i = 0; i < STRING_MAX_LENGTH; i++)
    {
        memData->driverName[i] = _pData->m_szDriverName[i];
        memData->kartId[i] = _pData->m_szKartID[i];
        memData->kartName[i] = _pData->m_szKartName[i];
        memData->category[i] = _pData->m_szCategory[i];
        memData->dash[i] = _pData->m_szDash[i];
        memData->trackId[i] = _pData->m_szTrackID[i];
        memData->trackName[i] = _pData->m_szTrackName[i];
    }

    memData->sequenceNumber++;
    mem.write();
}

void EventDeinit()
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = EGameState::MENU;
    memData->sequenceNumber++;
    mem.write();
}

void RunInit(SPluginsKartSession_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = EGameState::ONTRACK;

    for (int i = 0; i < STRING_MAX_LENGTH; i++)
        memData->setupFileName[i] = _pData->m_szSetupFileName[i];

    memData->sequenceNumber++;
    mem.write();
}

void RunDeinit()
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = EGameState::PIT;
    memData->sequenceNumber++;
    mem.write();
}

void RunStart()
{
    SSharedMemory_t *memData = mem.get();
    if (memData->gameState != EGameState::PAUSED)
        return;
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = prevGameState;
    memData->sequenceNumber++;
    mem.write();
}

void RunStop()
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    prevGameState = memData->gameState;
    memData->gameState = EGameState::PAUSED;
    memData->sequenceNumber++;
    mem.write();
}

void RunLap(SPluginsKartLap_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    // Last Lap
    memData->lap = _pData->m_iLapNum;
    memData->lastLapTime = _pData->m_iLapTime;
    memData->lastLapValid = _pData->m_iInvalid;

    // Best Lap
    if (memData->lastLapValid && (_pData->m_iPos <= bestLapPos || bestLapPos == -1))
    {
        memData->bestLapTime = _pData->m_iLapTime;
        bestLapPos = _pData->m_iPos;
    }

    // Estimated Time
    memData->estimatedLapTimeToLastLap = memData->lastLapTime;
    memData->estimatedLapTimeToBestLap = memData->bestLapTime;

    // Reset deltas
    memData->lastLapDeltaToLastLap = memData->lapDeltaToLastLap;
    memData->lastLapDeltaToBestLap = memData->lapDeltaToBestLap;
    memData->lapDeltaToLastLap = 0;
    memData->lapDeltaToBestLap = 0;

    memData->sequenceNumber++;
    mem.write();
}

void RunSplit(SPluginsKartSplit_t *_pData, int _iDataSize)
{
    // Maximum of 5 splits per track.
    if (_pData->m_iSplit >= MAX_SPLITS)
        return;

    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    // Last Split
    int lastSplit = memData->lastSplits[_pData->m_iSplit];
    memData->lastSplitIndex = _pData->m_iSplit;
    memData->lastSplits[_pData->m_iSplit] = _pData->m_iSplitTime;

    // Best Split
    int bestSplit = memData->bestSplits[_pData->m_iSplit];
    if (bestSplit == 0 || _pData->m_iSplitTime < bestSplit)
        memData->bestSplits[_pData->m_iSplit] = _pData->m_iSplitTime;

    // Deltas
    memData->lapDeltaToLastLap += lastSplit - _pData->m_iSplitTime;
    memData->lapDeltaToBestLap += _pData->m_iBestDiff;

    // Estimated Time
    memData->estimatedLapTimeToLastLap += lastSplit - _pData->m_iSplitTime;
    memData->estimatedLapTimeToBestLap += _pData->m_iBestDiff;

    memData->sequenceNumber++;
    mem.write();
}

void RunTelemetry(SPluginsKartData_t *_pData, int _iDataSize, float _fTime, float _fPos)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->time = _fTime;
    memData->trackPos = _fPos;
    memData->rpm = _pData->m_iRPM;
    memData->cylinderHeadTemperature = _pData->m_fCylinderHeadTemperature;
    memData->waterTemperature = _pData->m_fWaterTemperature;
    memData->gear = _pData->m_iGear;
    memData->fuel = _pData->m_fFuel;
    memData->speedometer = _pData->m_fSpeedometer;
    memData->posX = _pData->m_fPosX;
    memData->posY = _pData->m_fPosY;
    memData->posZ = _pData->m_fPosZ;
    memData->velocityX = _pData->m_fVelocityX;
    memData->velocityY = _pData->m_fVelocityY;
    memData->velocityZ = _pData->m_fVelocityZ;
    memData->accelerationX = _pData->m_fAccelerationX;
    memData->accelerationY = _pData->m_fAccelerationY;
    memData->accelerationZ = _pData->m_fAccelerationZ;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            memData->rot[i][j] = _pData->m_aafRot[i][j];

    memData->yaw = _pData->m_fYaw;
    memData->pitch = _pData->m_fPitch;
    memData->roll = _pData->m_fRoll;
    memData->yawVelocity = _pData->m_fYawVelocity;
    memData->pitchVelocity = _pData->m_fPitchVelocity;
    memData->rollVelocity = _pData->m_fRollVelocity;
    memData->inputSteer = _pData->m_fInputSteer;
    memData->inputThrottle = _pData->m_fInputThrottle;
    memData->inputBrake = _pData->m_fInputBrake;
    memData->inputFrontBrake = _pData->m_fInputFrontBrakes;
    memData->inputClutch = _pData->m_fInputClutch;

    for (int i = 0; i < 4; i++)
    {
        memData->wheelSpeed[i] = _pData->m_afWheelSpeed[i];
        memData->wheelMaterial[i] = _pData->m_aiWheelMaterial[i];
    }

    memData->steerTorque = _pData->m_fSteerTorque;
    memData->sequenceNumber++;
    mem.write();
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
    EGameState state = EGameState::CLOSED;
    if (_iState == 0)
        state = EGameState::ONTRACK;
    else if (_iState == 1)
        state = EGameState::SPECTATE;
    else if (_iState == 2)
        state = EGameState::REPLAY;

    SSharedMemory_t *memData = mem.get();
    if (memData->gameState != state && memData->gameState != EGameState::PAUSED)
    {
        memData->sequenceNumber++;
        mem.write();
        memData->gameState = state;
        memData->sequenceNumber++;
        mem.write();
    }

    *_piNumQuads = 0;
    *_piNumString = 0;
}

void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t *_pasSegment, float *_pfRaceData)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->numTrackSegments = _iNumSegments;
    for (int i = 0; i < _iNumSegments; i++)
    {
        STrackSegment_t *segment = &memData->trackSegments[i];
        segment->type = (ETrackSegmentType) _pasSegment[i].m_iType;
        segment->angle = _pasSegment[i].m_fAngle;
        segment->length = _pasSegment[i].m_fLength;
        segment->radius = _pasSegment[i].m_fRadius;
        segment->startX = _pasSegment[i].m_afStart[0];
        segment->startY = _pasSegment[i].m_afStart[1];
        segment->startZ = _pasSegment[i].m_fHeight;
    }

    for (int i = 0; i < MAX_SPLITS + 2; i++)
        memData->racedata[i] = _pfRaceData[i];

    memData->sequenceNumber++;
    mem.write();
}

int SpectateVehicles(int _iNumVehicles, SPluginsSpectateVehicle_t *_pVehicleData, int _iCurSelection, int *_piSelect)
{
    return 0;
}

int SpectateCameras(int _iNumCameras, char *_pCameraData, int _iCurSelection, int *_piSelect)
{
    return 0;
}