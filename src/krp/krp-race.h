#ifndef KRP_RACE_H
#define KRP_RACE_H

/******************************************************************************
structures and functions to receive the race data
******************************************************************************/

namespace krp
{
    namespace race
    {
        typedef struct
        {
            int m_iType; /* -1 = replay; 1 = testing; 2 = race; 4 = challenge */
            char m_szName[100];
            char m_szTrackName[100];
            float m_fTrackLength; /* meters */
        } SPluginsRaceEvent_t;

        typedef struct
        {
            int m_iRaceNum; /* unique race number */
            char m_szName[100];
            char m_szKartName[100];
            char m_szKartShortName[100];
            char m_szCategory[100];
            int m_iUnactive; /* if set to 1, the driver left the event and the following fields are not set */
            int m_iNumberOfGears;
            int m_iMaxRPM;
        } SPluginsRaceAddEntry_t;

        typedef struct
        {
            int m_iRaceNum; /* race number */
        } SPluginsRaceRemoveEntry_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iGroup1, m_iGroup2; /* 0 = A, 1 = B, 2 = C, ... Only used for Qualify Heats */
            int m_iSessionState;      /* testing / waiting: always 0. practice / qualify / warmup: 16 = in progress; 32 = completed. qualify heat / second chance heat / prefinal / final: 16 = in progress; 32 = semaphore; 64 = sighting lap; 128 = warmup lap; 256 = pre-start; 512 = race over; 1024 = completed; 2048 = rolling start */
            int m_iSessionLength;     /* milliseconds. 0 = no limit */
            int m_iSessionNumLaps;
            int m_iNumEntries;
            int m_aiEntries[50];
            int m_aiGrid[50];
            int m_iConditions;         /* 0 = sunny; 1 = cloudy; 2 = rainy */
            float m_fAirTemperature;   /* degrees Celsius */
            float m_fTrackTemperature; /* degrees Celsius */
        } SPluginsRaceSession_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iSessionState;  /* testing / waiting: always 0. practice / qualify / warmup: 16 = in progress; 32 = completed. qualify heat / second chance heat / prefinal / final: 16 = in progress; 32 = semaphore; 64 = sighting lap; 128 = warmup lap; 256 = pre-start; 512 = race over; 1024 = completed; 2048 = rolling start */
            int m_iSessionLength; /* milliseconds. 0 = no limit */
        } SPluginsRaceSessionState_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iRaceNum; /* race number */
            int m_iLapNum;  /* lap index */
            int m_iInvalid;
            int m_iLapTime;   /* milliseconds */
            int m_aiSplit[2]; /* milliseconds */
            float m_fSpeed;   /* meters/second */
            int m_iBest;      /* 1 = personal best lap; 2 = overall best lap */
        } SPluginsRaceLap_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iRaceNum;   /* race number */
            int m_iLapNum;    /* lap index */
            int m_iSplit;     /* split index */
            int m_iSplitTime; /* milliseconds */
        } SPluginsRaceSplit_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iRaceNum; /* race number */
            int m_iLapNum;  /* lap index */
            float m_fSpeed; /* meters/second */
        } SPluginsRaceSpeed_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iRaceNum;       /* race number */
            int m_iCommunication; /* 1 = change state; 2 = penalty */
            int m_iState;         /* 1 = DNS; 2 = retired; 3 = DSQ */
            int m_iReason;        /* Reason for DSQ. 0 = jump start; 1 = too many offences; 3 = rolling start speeding; 4 = rolling start too slow; 5 = rolling start corridor crossing; 6 = rolling start overtaking; 7 = director */
            int m_iOffence;       /* 1 = jump start; 3 = cutting; 4 = rolling start speeding; 5 = rolling start too slow; 6 = rolling start corridor crossing; 7 = rolling start overtaking */
            int m_iLap;           /* lap index */
            int m_iType;          /* 1 = time penalty; 2 = position penalty */
            int m_iTime;          /* milliseconds. Penalty time */
        } SPluginsRaceCommunication_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iSessionState; /* testing / waiting: always 0. practice / qualify / warmup: 16 = in progress; 32 = completed. qualify heat / second chance heat / prefinal / final: 16 = in progress; 32 = semaphore; 64 = sighting lap; 128 = warmup lap; 256 = pre-start; 512 = race over; 1024 = completed; 2048 = rolling start */
            int m_iSessionTime;  /* milliseconds. Current session time */
            int m_iNumEntries;   /* number of entries */
        } SPluginsRaceClassification_t;

        typedef struct
        {
            int m_iRaceNum;     /* race number */
            int m_iState;       /* 1 = DNS; 2 = retired; 3 = DSQ */
            int m_iBestLap;     /* milliseconds */
            float m_fBestSpeed; /* meters/second */
            int m_iBestLapNum;  /* best lap index */
            int m_iNumLaps;     /* number of laps */
            int m_iGap;         /* milliseconds */
            int m_iGapLaps;
            int m_iPenalty; /* milliseconds */
            int m_iPit;     /* 0 = on track; 1 = in the pits */
        } SPluginsRaceClassificationEntry_t;

        typedef struct
        {
            int m_iRaceNum;                  /* race number */
            float m_fPosX, m_fPosY, m_fPosZ; /* meters */
            float m_fYaw;                    /* angle from north. degrees */
            float m_fTrackPos;               /* position on the centerline, from 0 to 1 */
        } SPluginsRaceTrackPosition_t;

        typedef struct
        {
            int m_iRaceNum;       /* race number */
            int m_iActive;        /* if set to 0, the vehicle is not active and the following fields are not set */
            int m_iRPM;           /* engine RPM */
            int m_iGear;          /* 0 = Neutral */
            float m_fSpeedometer; /* meters/second */
            float m_fSteer;       /* -1 ( left ) to 1 ( right ) */
            float m_fThrottle;    /* 0 to 1 */
            float m_fBrake;       /* 0 to 1 */
        } SPluginsRaceVehicleData_t;
    }
}

#endif