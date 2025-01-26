#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>

std::string SHARED_MEMORY_NAME = "$krp$";

const int STRING_MAX_LENGTH = 100;
const int MAX_TRACK_SEGMENTS = 100;
const int MAX_ENTRIES = 50;
const int MAX_SPLITS = 2;

enum EGameState
{
    CLOSED = 0,
    MENU,
    PIT,
    PAUSED,
    ONTRACK,
    SPECTATE,
    REPLAY
};

enum EDriveType
{
    DIRECT = 0,
    CLUTCH = 1,
    SHIFTER = 2
};

enum EEngineCooling
{
    AIRCOOLED = 0,
    WATERCOOLED = 1
};

enum ETrackSegmentType
{
    STRAIGHT = 0,
    CURVE
};

enum EEventType
{
    REPLAY = -1,
    NONE = 0,
    TESTING,
    RACE,
    CHALLENGE = 4
};

enum EWeatherCondition {
    SUNNY = 0,
    CLOUDY,
    RAINY
};

typedef struct
{
    ETrackSegmentType type;
    float length;
    float radius;
    float angle;
    float startX;
    float startY;
    float startZ;
} STrackSegment_t;

typedef struct
{
    int raceNumber;
    char name[STRING_MAX_LENGTH];
    char kartName[STRING_MAX_LENGTH];
    char kartShortName[STRING_MAX_LENGTH];
    char category[STRING_MAX_LENGTH];
    bool unactive;
    int numberOfGears;
    int maxRPM;
} SEventEntry_t;

typedef struct
{
    int sequenceNumber;
    EGameState gameState;

    char driverName[STRING_MAX_LENGTH];
    char kartId[STRING_MAX_LENGTH];
    char kartName[STRING_MAX_LENGTH];
    EDriveType driveType;
    int numberOfGears;
    int maxRpm;
    int limiter;
    int shiftRpm;
    EEngineCooling engineCooling;
    float engineOptTemperature;
    float engineTemperatureAlarm[2];
    float maxFuel;
    char category[STRING_MAX_LENGTH];
    char dash[STRING_MAX_LENGTH];

    char setupFileName[STRING_MAX_LENGTH];

    float time;
    float trackPos;
    int rpm;
    float cylinderHeadTemperature;
    float waterTemperature;
    int gear;
    float fuel;
    float speedometer;
    float posX, posY, posZ;
    float velocityX, velocityY, velocityZ;
    float accelerationX, accelerationY, accelerationZ;
    float rot[3][3];
    float yaw, pitch, roll;
    float yawVelocity, pitchVelocity, rollVelocity;
    float inputSteer;
    float inputThrottle;
    float inputBrake;
    float inputFrontBrake;
    float inputClutch;
    float wheelSpeed[4];
    int wheelMaterial[4];
    float steerTorque;

    int lap;
    int lastLapTime;
    bool lastLapValid;
    int bestLapTime;
    int estimatedLapTimeToLastLap;
    int estimatedLapTimeToBestLap;
    int lastLapDeltaToLastLap;
    int lastLapDeltaToBestLap;
    int lapDeltaToLastLap;
    int lapDeltaToBestLap;

    int lastSplitIndex;
    int lastSplits[MAX_SPLITS];
    int bestSplits[MAX_SPLITS];

    char trackId[STRING_MAX_LENGTH];
    char trackName[STRING_MAX_LENGTH];
    float trackLength;
    float racedata[MAX_SPLITS + 2];
    int numTrackSegments;
    STrackSegment_t trackSegments[MAX_TRACK_SEGMENTS];

    EEventType eventType;
    char eventName[STRING_MAX_LENGTH];
    int numEventEntries;
    SEventEntry_t eventEntries[MAX_ENTRIES];

    int session;
    int sessionSeries;
    int sessionGroups[2];
    int sessionState;
    int sessionLength;
    int sessionLaps;
    int numSessionEntries;
    int sessionEntries[MAX_ENTRIES];
    int sessionGrid[MAX_ENTRIES];
    EWeatherCondition weatherCondition;
    float airTemperature;
    float trackTemperature;

    int 
} SSharedMemory_t;

#endif