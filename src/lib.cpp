#include <iostream>
#include <vector>
#include <map>
#include "lib.h"
#include "memory/memory.cpp"

Memory<SSharedMemory_t> mem(SHARED_MEMORY_NAME);
EGameState prevGameState = EGameState::CLOSED;
std::map<int, int> entries;

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
    mem.create();
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber = 0;
    memData->sequenceNumber++;
    mem.write();
    memData->version = SHARED_MEMORY_VERSION;
    memData->gameState = EGameState::MENU;
    memData->sequenceNumber++;
    mem.write();

    /*
    return value is requested rate
    0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
    */
    return 0;
}

void Shutdown()
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();
    memData->gameState = EGameState::CLOSED;
    memData->sequenceNumber++;
    mem.write();
    mem.close();
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
    entries.clear();
    memData->gameState = EGameState::MENU;

    // Reset laps
    for (int i = 0; i < MAX_ENTRIES; i++)
        memData->kartIdxLap[i] = 0;

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
    memData->lastLapValid = !((bool)_pData->m_iInvalid);

    // Best Lap
    if (memData->lastLapValid && (memData->lastLapTime <= memData->bestLapTime || memData->bestLapTime == 0))
        memData->bestLapTime = memData->lastLapTime;

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
    memData->lapDeltaToBestLap += bestSplit - _pData->m_iSplitTime;

    // Estimated Time
    memData->estimatedLapTimeToLastLap += lastSplit - _pData->m_iSplitTime;
    memData->estimatedLapTimeToBestLap += bestSplit - _pData->m_iSplitTime;

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
        state = EGameState::SPECTATING;
    else if (_iState == 2)
        state = EGameState::REPLAYING;

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
        segment->type = (ETrackSegmentType)_pasSegment[i].m_iType;
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

void RaceEvent(SPluginsRaceEvent_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->eventType = (EEventType)_pData->m_iType;
    for (int i = 0; i < STRING_MAX_LENGTH; i++)
        memData->eventName[i] = _pData->m_szName[i];

    memData->sequenceNumber++;
    mem.write();
}

void RaceDeinit()
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->eventType = EEventType::NONE;
    for (int i = 0; i < STRING_MAX_LENGTH; i++)
        memData->eventName[i] = '\0';

    memData->sequenceNumber++;
    mem.write();
}

void RaceAddEntry(SPluginsRaceAddEntry_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    auto entry = entries.find(_pData->m_iRaceNum);
    if (entry != entries.end())
    {
        SEventEntry_t *newEntry = &memData->eventEntries[entry->second];
        newEntry->raceNumber = _pData->m_iRaceNum;
        newEntry->unactive = _pData->m_iUnactive;
        newEntry->numberOfGears = _pData->m_iNumberOfGears;
        newEntry->maxRPM = _pData->m_iMaxRPM;

        for (int i = 0; i < STRING_MAX_LENGTH; i++)
        {
            newEntry->name[i] = _pData->m_szName[i];
            newEntry->kartName[i] = _pData->m_szKartName[i];
            newEntry->kartShortName[i] = _pData->m_szKartShortName[i];
            newEntry->category[i] = _pData->m_szCategory[i];
        }
    }
    else
    {
        memData->numEventEntries++;
        entries.insert(std::pair<int, int>(_pData->m_iRaceNum, memData->numEventEntries - 1));

        SEventEntry_t *newEntry = &memData->eventEntries[memData->numEventEntries - 1];
        newEntry->raceNumber = _pData->m_iRaceNum;
        newEntry->unactive = _pData->m_iUnactive;
        newEntry->numberOfGears = _pData->m_iNumberOfGears;
        newEntry->maxRPM = _pData->m_iMaxRPM;

        for (int i = 0; i < STRING_MAX_LENGTH; i++)
        {
            newEntry->name[i] = _pData->m_szName[i];
            newEntry->kartName[i] = _pData->m_szKartName[i];
            newEntry->kartShortName[i] = _pData->m_szKartShortName[i];
            newEntry->category[i] = _pData->m_szCategory[i];
        }
    }

    memData->sequenceNumber++;
    mem.write();
}

void RaceRemoveEntry(SPluginsRaceRemoveEntry_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    auto entry = entries.find(_pData->m_iRaceNum);
    if (entry == entries.end())
        return;

    SEventEntry_t *newEntry = &memData->eventEntries[entry->second];
    newEntry->raceNumber = _pData->m_iRaceNum;
    newEntry->unactive = true;

    memData->sequenceNumber++;
    mem.write();
}

void RaceSession(SPluginsRaceSession_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->session = _pData->m_iSession;
    memData->sessionSeries = _pData->m_iSessionSeries;
    memData->sessionGroups[0] = _pData->m_iGroup1;
    memData->sessionGroups[1] = _pData->m_iGroup2;
    memData->sessionState = _pData->m_iSessionState;
    memData->sessionLength = _pData->m_iSessionLength;
    memData->sessionLaps = _pData->m_iSessionNumLaps;
    memData->numSessionEntries = _pData->m_iNumEntries;
    memData->weatherCondition = (EWeatherCondition)_pData->m_iConditions;
    memData->airTemperature = _pData->m_fAirTemperature;
    memData->trackTemperature = _pData->m_fTrackTemperature;

    for (int i = 0; i < MAX_ENTRIES; i++)
    {
        memData->sessionEntries[i] = _pData->m_aiEntries[i];
        memData->sessionGrid[i] = _pData->m_aiGrid[i];
    }

    // Resets
    memData->lap = 0;
    memData->lastLapTime = 0;
    memData->lastLapValid = false;
    memData->bestLapTime = 0;
    memData->estimatedLapTimeToLastLap = 0;
    memData->estimatedLapTimeToBestLap = 0;
    memData->lastLapDeltaToLastLap = 0;
    memData->lastLapDeltaToBestLap = 0;
    memData->lapDeltaToLastLap = 0;
    memData->lapDeltaToBestLap = 0;

    memData->lastSplitIndex = 0;
    for (int i = 0; i < MAX_SPLITS; i++) {
        memData->lastSplits[i] = 0;
        memData->bestSplits[i] = 0;
    }

    for (int i = 0; i < MAX_ENTRIES; i++) {
        memData->kartIdxLap[i] = 0;
        memData->kartIdxLastLapTime[i] = 0;
        memData->kartIdxLastLapValid[i] = false;
        memData->kartIdxBestLapTime[i] = 0;
        memData->kartIdxEstimatedLapTimeToLastLap[i] = 0;
        memData->kartIdxEstimatedLapTimeToBestLap[i] = 0;
        memData->kartIdxLastLapDeltaToLastLap[i] = 0;
        memData->kartIdxLastLapDeltaToBestLap[i] = 0;
        memData->kartIdxLapDeltaToLastLap[i] = 0;
        memData->kartIdxLapDeltaToBestLap[i] = 0;

        memData->kartIdxLastSplitIndex[i] = 0;
        for (int j = 0; j < MAX_SPLITS; j++) {
            memData->kartIdxLastSplits[i][j] = 0;
            memData->kartIdxBestSplits[i][j] = 0;
        }

        memData->kartIdxLastSpeed[i] = 0;
        memData->kartIdxBestSpeed[i] = 0;
    }

    memData->numTrackPositions = 0;

    memData->sequenceNumber++;
    mem.write();
}

void RaceSessionState(SPluginsRaceSessionState_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->session = _pData->m_iSession;
    memData->sessionSeries = _pData->m_iSessionSeries;
    memData->sessionState = _pData->m_iSessionState;
    memData->sessionLength = _pData->m_iSessionLength;

    memData->sequenceNumber++;
    mem.write();
}

void RaceLap(SPluginsRaceLap_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    int id = entries.find(_pData->m_iRaceNum)->second;

    // Last Lap
    memData->kartIdxLap[id] = _pData->m_iLapNum;
    memData->kartIdxLastLapTime[id] = _pData->m_iLapTime;
    memData->kartIdxLastLapValid[id] = !((bool)_pData->m_iInvalid);

    // Best Event Lap
    if (memData->kartIdxLastLapValid[id] && (memData->kartIdxLastLapTime[id] <= memData->bestEventLapTime || memData->bestEventLapTime == 0))
        memData->bestEventLapTime = memData->kartIdxLastLapTime[id];

    // Best Session Lap
    if (memData->kartIdxLastLapValid[id] && (memData->kartIdxLastLapTime[id] <= memData->bestSessionLapTime || memData->bestSessionLapTime == 0))
        memData->bestSessionLapTime = memData->kartIdxLastLapTime[id];

    // Best Lap
    if (memData->kartIdxLastLapValid[id] && (memData->kartIdxLastLapTime[id] <= memData->kartIdxBestLapTime[id] || memData->kartIdxBestLapTime[id] == 0))
        memData->kartIdxBestLapTime[id] = memData->kartIdxLastLapTime[id];

    // Estimated Time
    memData->kartIdxEstimatedLapTimeToLastLap[id] = memData->kartIdxLastLapTime[id];
    memData->kartIdxEstimatedLapTimeToBestLap[id] = memData->kartIdxBestLapTime[id];

    // Reset deltas
    memData->kartIdxLastLapDeltaToLastLap[id] = memData->kartIdxLapDeltaToLastLap[id];
    memData->kartIdxLastLapDeltaToBestLap[id] = memData->kartIdxLapDeltaToBestLap[id];
    memData->kartIdxLapDeltaToLastLap[id] = 0;
    memData->kartIdxLapDeltaToBestLap[id] = 0;

    memData->sequenceNumber++;
    mem.write();
}

void RaceSplit(SPluginsRaceSplit_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    int id = entries.find(_pData->m_iRaceNum)->second;

    // Last Split
    int lastSplit = memData->kartIdxLastSplits[id][_pData->m_iSplit];
    memData->kartIdxLastSplitIndex[id] = _pData->m_iSplit;
    memData->kartIdxLastSplits[id][_pData->m_iSplit] = _pData->m_iSplitTime;

    // Best Split
    int bestSplit = memData->kartIdxBestSplits[id][_pData->m_iSplit];
    if (bestSplit == 0 || _pData->m_iSplitTime < bestSplit)
        memData->kartIdxBestSplits[id][_pData->m_iSplit] = _pData->m_iSplitTime;

    // Deltas
    memData->kartIdxLapDeltaToLastLap[id] += lastSplit - _pData->m_iSplitTime;
    memData->kartIdxLapDeltaToBestLap[id] += bestSplit - _pData->m_iSplitTime;

    // Estimated Time
    memData->kartIdxEstimatedLapTimeToLastLap[id] += lastSplit - _pData->m_iSplitTime;
    memData->kartIdxEstimatedLapTimeToBestLap[id] += bestSplit - _pData->m_iSplitTime;

    memData->sequenceNumber++;
    mem.write();
}

void RaceSpeed(SPluginsRaceSpeed_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    int id = entries.find(_pData->m_iRaceNum)->second;

    // Last Speed
    memData->kartIdxLastSpeed[id] = _pData->m_fSpeed;

    // Best Speed
    if (_pData->m_fSpeed > memData->kartIdxBestSpeed[id] || memData->kartIdxBestSpeed[id] == 0)
        memData->kartIdxBestSpeed[id] = _pData->m_fSpeed;

    memData->sequenceNumber++;
    mem.write();
}

void RaceCommunication(SPluginsRaceCommunication_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->numCommunications++;

    SCommunication_t *comm = &memData->communications[memData->numCommunications - 1];
    comm->lap = _pData->m_iLap;
    comm->time = _pData->m_iTime;
    comm->raceNumber = _pData->m_iRaceNum;
    comm->session = _pData->m_iSession;
    comm->sessionSeries = _pData->m_iSessionSeries;
    comm->type = (ECommunicationType)_pData->m_iCommunication;
    comm->state = (EEntryState)_pData->m_iState;
    comm->reason = (ECommunicationReason)_pData->m_iReason;
    comm->offence = (ECommunicationOffence)_pData->m_iOffence;
    comm->penaltyType = (ECommunicationPenaltyType)_pData->m_iType;

    memData->sequenceNumber++;
    mem.write();
}

void RaceClassification(SPluginsRaceClassification_t *_pData, int _iDataSize, SPluginsRaceClassificationEntry_t *_pArray, int _iElemSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->classification.session = _pData->m_iSession;
    memData->classification.sessionSeries = _pData->m_iSessionSeries;
    memData->classification.sessionState = _pData->m_iSessionState;
    memData->classification.sessionTime = _pData->m_iSessionTime;
    memData->classification.numEntries = _pData->m_iNumEntries;

    for (int i = 0; i < _pData->m_iNumEntries; i++)
    {
        SClassificationEntry_t entry;
        entry.raceNumber = _pArray[i].m_iRaceNum;
        entry.state = (EEntryState)_pArray[i].m_iState;
        entry.bestLapTime = _pArray[i].m_iBestLap;
        entry.bestLapNum = _pArray[i].m_iBestLapNum;
        entry.numLaps = _pArray[i].m_iNumLaps;
        entry.gap = _pArray[i].m_iGap;
        entry.gapLaps = _pArray[i].m_iGapLaps;
        entry.penalty = _pArray[i].m_iPenalty;
        entry.inPit = _pArray[i].m_iPit;
        memData->classification.entries[i] = entry;
    }

    memData->sequenceNumber++;
    mem.write();
}

void RaceTrackPosition(int _iNumVehicles, SPluginsRaceTrackPosition_t *_pArray, int _iElemSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->numTrackPositions = _iNumVehicles;

    for (int i = 0; i < _iNumVehicles; i++)
    {
        STrackPosition_t *pos = &memData->trackPositions[i];
        pos->raceNumber = _pArray[i].m_iRaceNum;
        pos->posX = _pArray[i].m_fPosX;
        pos->posY = _pArray[i].m_fPosY;
        pos->posZ = _pArray[i].m_fPosZ;
        pos->yaw = _pArray[i].m_fYaw;
        pos->trackPos = _pArray[i].m_fTrackPos;
    }

    memData->sequenceNumber++;
    mem.write();
}

void RaceVehicleData(SPluginsRaceVehicleData_t *_pData, int _iDataSize)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    int id = entries.find(_pData->m_iRaceNum)->second;

    SVehicleData_t *data = &memData->kartIdxVehicleData[id];
    data->raceNumber = _pData->m_iRaceNum;
    data->active = _pData->m_iActive;
    data->rpm = _pData->m_iRPM;
    data->gear = _pData->m_iGear;
    data->speed = _pData->m_fSpeedometer;
    data->steer = _pData->m_fSteer;
    data->throttle = _pData->m_fThrottle;
    data->brake = _pData->m_fBrake;

    memData->sequenceNumber++;
    mem.write();
}

int SpectateVehicles(int _iNumVehicles, SPluginsSpectateVehicle_t *_pVehicleData, int _iCurSelection, int *_piSelect)
{
    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->selectedSpectateVehicle = _iCurSelection;
    memData->numSpectateVehicles = _iNumVehicles;

    for (int i = 0; i < _iNumVehicles; i++)
    {
        SSpectateVehicle_t *vehicle = &memData->spectateVehicles[i];
        vehicle->raceNumber = _pVehicleData[i].m_iRaceNum;
        for (int j = 0; j < STRING_MAX_LENGTH; j++)
            vehicle->name[j] = _pVehicleData[i].m_szName[j];
    }

    int returnVal = 0;
    if (memData->requestedSpectateVehicle != memData->selectedSpectateVehicle && memData->requestedSpectateVehicle >= 0 && memData->requestedSpectateVehicle < _iNumVehicles)
    {
        *(_piSelect) = memData->requestedSpectateVehicle;
        memData->requestedSpectateVehicle = -1;
        returnVal = 1;
    }

    memData->sequenceNumber++;
    mem.write();

    return returnVal;
}

int SpectateCameras(int _iNumCameras, char *_pCameraData, int _iCurSelection, int *_piSelect)
{
    char *array = (char *)_pCameraData;

    SSharedMemory_t *memData = mem.get();
    memData->sequenceNumber++;
    mem.write();

    memData->selectedCamera = _iCurSelection;
    memData->numCameras = _iNumCameras;

    for (int i = 0; i < _iNumCameras; i++)
    {
        strcpy_s(memData->cameras[i], STRING_MAX_LENGTH, array);
        array += strlen(array) + 1;
    }

    int returnVal = 0;
    if (memData->requestedCamera != memData->selectedCamera && memData->requestedCamera >= 0 && memData->requestedCamera < _iNumCameras)
    {
        *(_piSelect) = memData->requestedCamera;
        memData->requestedCamera = -1;
        returnVal = 1;
    }

    memData->sequenceNumber++;
    mem.write();

    return returnVal;
}