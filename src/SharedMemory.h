#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>

const int SHARED_MEMORY_VERSION = 1;
std::string SHARED_MEMORY_NAME = "$krp$";

const int STRING_MAX_LENGTH = 100;
const int MAX_COMMUNICATIONS = 100;
const int MAX_TRACK_SEGMENTS = 100;
const int MAX_ENTRIES = 50;
const int MAX_CAMERAS = 20;
const int MAX_SPLITS = 2;

enum EGameState
{
    CLOSED = 0,
    MENU,
    PIT,
    PAUSED,
    ONTRACK,
    SPECTATING,
    REPLAYING
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

enum EWeatherCondition
{
    SUNNY = 0,
    CLOUDY,
    RAINY
};

enum ECommunicationType
{
    CHANGE_STATE = 1,
    PENALTY
};

enum ECommunicationReason
{
    JUMP_START_REASON = 0,
    TOO_MANY_OFFENCES,
    ROLLING_START_SPEEDING,
    ROLLING_START_TOO_SLOW,
    ROLLING_START_CORRIDOR_CROSSING,
    ROLLING_START_OVERTAKING,
    DIRECTOR
};

enum ECommunicationOffence
{
    JUMP_START_OFFENCE = 1,
    CUTTING_OFFENCE = 3,
    ROLLING_START_SPEEDING_OFFENCE,
    ROLLING_START_TOO_SLOW_OFFENCE,
    ROLLING_START_CORRIDOR_CROSSING_OFFENCE,
    ROLLING_START_OVERTAKING_OFFENCE
};

enum ECommunicationPenaltyType
{
    TIME = 1,
    POSITION
};

enum EEntryState
{
    DNS = 1,
    RETIRED,
    DSQ
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
    int lap;
    int time;
    int raceNumber;
    int session;
    int sessionSeries;
    ECommunicationType type;
    EEntryState state;
    ECommunicationReason reason;
    ECommunicationOffence offence;
    ECommunicationPenaltyType penaltyType;
} SCommunication_t;

typedef struct
{
    int raceNumber;
    float posX, posY, posZ;
    float yaw;
    float trackPos;
} STrackPosition_t;

typedef struct
{
    int raceNumber;
    bool active;
    int rpm;
    int gear;
    float speed;
    float steer;
    float throttle;
    float brake;
} SVehicleData_t;

typedef struct
{
    int raceNumber;
    EEntryState state;
    int bestLapTime;
    float bestSpeed;
    int bestLapNum;
    int numLaps;
    int gap;
    int gapLaps;
    int penalty;
    bool inPit;
} SClassificationEntry_t;

typedef struct
{
    int session;
    int sessionSeries;
    int sessionState;
    int sessionTime;
    int numEntries;
    SClassificationEntry_t entries[MAX_ENTRIES];
} SClassification_t;

typedef struct
{
    int raceNumber;
    char name[STRING_MAX_LENGTH];
} SSpectateVehicle_t;

typedef struct
{
    int sequenceNumber;
    int version;
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

    int bestEventLapTime;
    int bestSessionLapTime;

    int kartIdxLap[MAX_ENTRIES];
    int kartIdxLastLapTime[MAX_ENTRIES];
    bool kartIdxLastLapValid[MAX_ENTRIES];
    int kartIdxBestLapTime[MAX_ENTRIES];
    int kartIdxEstimatedLapTimeToLastLap[MAX_ENTRIES];
    int kartIdxEstimatedLapTimeToBestLap[MAX_ENTRIES];
    int kartIdxLastLapDeltaToLastLap[MAX_ENTRIES];
    int kartIdxLastLapDeltaToBestLap[MAX_ENTRIES];
    int kartIdxLapDeltaToLastLap[MAX_ENTRIES];
    int kartIdxLapDeltaToBestLap[MAX_ENTRIES];

    int kartIdxLastSplitIndex[MAX_ENTRIES];
    int kartIdxLastSplits[MAX_ENTRIES][MAX_SPLITS];
    int kartIdxBestSplits[MAX_ENTRIES][MAX_SPLITS];

    float kartIdxLastSpeed[MAX_ENTRIES];
    float kartIdxBestSpeed[MAX_ENTRIES];

    int numCommunications;
    SCommunication_t communications[MAX_ENTRIES];

    int numTrackPositions;
    STrackPosition_t trackPositions[MAX_ENTRIES];

    SVehicleData_t kartIdxVehicleData[MAX_ENTRIES];
    SClassification_t classification;

    int requestedSpectateVehicle;
    int selectedSpectateVehicle;
    int numSpectateVehicles;
    SSpectateVehicle_t spectateVehicles[MAX_ENTRIES];

    int requestedCamera;
    int selectedCamera;
    int numCameras;
    char cameras[MAX_CAMERAS][STRING_MAX_LENGTH];
} SSharedMemory_t;

#endif