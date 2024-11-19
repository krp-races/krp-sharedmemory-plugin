#ifndef KRP_TRACK_H
#define KRP_TRACK_H

/******************************************************************************
structures and functions to receive the track center line
******************************************************************************/

namespace krp
{
    namespace track
    {
        typedef struct
        {
            int m_iType;        /* 0 = straight; 1 = curve */
            float m_fLength;    /* meters */
            float m_fRadius;    /* curve radius in meters. < 0 for left curves; 0 for straights */
            float m_fAngle;     /* start angle in degrees. 0 = north */
            float m_afStart[2]; /* start position in meters */
            float m_fHeight;    /* start height in meters */
        } SPluginsTrackSegment_t;
    }
}

#endif