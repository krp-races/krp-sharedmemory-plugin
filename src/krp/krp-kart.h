#ifndef KRP_KART_H
#define KRP_KART_H

/******************************************************************************
structures and functions to receive data from the simulated kart
******************************************************************************/

namespace krp
{
    namespace kart
    {
        typedef struct
        {
            char m_szDriverName[100];
            char m_szKartID[100];
            char m_szKartName[100];
            int m_iDriveType; /* 0 = direct; 1 = clutch; 2 = shifter */
            int m_iNumberOfGears;
            int m_iMaxRPM;
            int m_iLimiter;
            int m_iShiftRPM;
            int m_iEngineCooling;                /* 0 = aircooled; 1 = watercooled */
            float m_fEngineOptTemperature;       /* degrees Celsius */
            float m_afEngineTemperatureAlarm[2]; /* degrees Celsius. Lower and upper limits */
            float m_fMaxFuel;                    /* liters */
            char m_szCategory[100];
            char m_szDash[100];
            char m_szTrackID[100];
            char m_szTrackName[100];
            float m_fTrackLength; /* centerline length. meters */
            int m_iType;          /* 1 = testing; 2 = race; 4 = challenge */
        } SPluginsKartEvent_t;

        typedef struct
        {
            int m_iSession; /* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
            int m_iSessionSeries;
            int m_iConditions;         /* 0 = sunny; 1 = cloudy; 2 = rainy */
            float m_fAirTemperature;   /* degrees Celsius */
            float m_fTrackTemperature; /* degrees Celsius */
            char m_szSetupFileName[100];
        } SPluginsKartSession_t;

        typedef struct
        {
            int m_iRPM;                                                 /* engine rpm */
            float m_fCylinderHeadTemperature;                           /* degrees Celsius */
            float m_fWaterTemperature;                                  /* degrees Celsius */
            int m_iGear;                                                /* 0 = Neutral */
            float m_fFuel;                                              /* liters */
            float m_fSpeedometer;                                       /* meters/second */
            float m_fPosX, m_fPosY, m_fPosZ;                            /* world position of a reference point attached to chassis ( not CG ) */
            float m_fVelocityX, m_fVelocityY, m_fVelocityZ;             /* velocity of CG in world coordinates. meters/second */
            float m_fAccelerationX, m_fAccelerationY, m_fAccelerationZ; /* acceleration of CG local to chassis rotation, expressed in G ( 9.81 m/s2 ) and averaged over the latest 10ms */
            float m_aafRot[3][3];                                       /* rotation matrix of the chassis */
            float m_fYaw, m_fPitch, m_fRoll;                            /* degrees, -180 to 180 */
            float m_fYawVelocity, m_fPitchVelocity, m_fRollVelocity;    /* degress / second */
            float m_fInputSteer;                                        /* degrees. Negative = left */
            float m_fInputThrottle;                                     /* 0 to 1 */
            float m_fInputBrake;                                        /* 0 to 1 */
            float m_fInputFrontBrakes;                                  /* 0 to 1 */
            float m_fInputClutch;                                       /* 0 to 1. 0 = Fully engaged */
            float m_afWheelSpeed[4];                                    /* meters/second. 0 = front-left; 1 = front-right; 2 = rear-left; 3 = rear-right */
            int m_aiWheelMaterial[4];                                   /* material index. 0 = not in contact */
            float m_fSteerTorque;                                       /* Nm */
        } SPluginsKartData_t;

        typedef struct
        {
            int m_iLapNum; /* lap index */
            int m_iInvalid;
            int m_iLapTime; /* milliseconds */
            int m_iPos;     /* 1 = best lap */
        } SPluginsKartLap_t;

        typedef struct
        {
            int m_iSplit;     /* split index */
            int m_iSplitTime; /* milliseconds */
            int m_iBestDiff;  /* milliseconds. Difference with best lap */
        } SPluginsKartSplit_t;
    }
}

#endif