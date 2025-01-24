#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>

// Constants

std::string SHARED_MEMORY_KARTDATA_NAME = "$krp_kartdata$";
std::string SHARED_MEMORY_TRACKDATA_NAME = "$krp_trackdata$";

const int STRING_MAX_LENGTH = 100;
const int MAX_SPLITS = 5;
const int MAX_TRACK_SEGMENTS = 100;

// Enumerations

enum EGameState
{
    ONTRACK = 0,
    SPECTATE,
    REPLAY
};

enum EDriveType
{
    DIRECT = 0,
    CLUTCH,
    SHIFTER
};

enum ECoolingType
{
    AIRCOOLED = 0,
    WATERCOOLED
};

enum ETrackSegmentType
{
    STRAIGHT = 0,
    CURVE
};

// Types

typedef struct
{
    int sequenceNumber;

    // Status
    EGameState state;
    bool driving;
    bool paused;

    // Data
    char driverName[STRING_MAX_LENGTH];
    char kartID[STRING_MAX_LENGTH];
    char kartName[STRING_MAX_LENGTH];
    EDriveType driveType;
    int numberOfGears;
    int maxRPM;
    int limiter;
    int shiftRPM;
    ECoolingType coolingType;
    float engineOptTemperature;
    float engineTemperatureAlarm[2];
    float maxFuel;
    char category[STRING_MAX_LENGTH];
    char dash[STRING_MAX_LENGTH];
    char trackId[STRING_MAX_LENGTH];
    char trackName[STRING_MAX_LENGTH];
    float trackLength;

    // Session
    char setupFileName[STRING_MAX_LENGTH];

    // Telemetry
    float driveTime;
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
    float inputFrontBrakes;
    float inputClutch;
    float wheelSpeed[4];
    int wheelMaterial[4];
    float steerTorque;

    // Lap
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

    // Split
    int lastSplitIndex;
    int lastSplits[MAX_SPLITS];
    int bestSplits[MAX_SPLITS];
} SKartData_t;

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
    int sequenceNumber;
    int numSegments;
    STrackSegment_t segments[MAX_TRACK_SEGMENTS];
} STrackData_t;

#endif