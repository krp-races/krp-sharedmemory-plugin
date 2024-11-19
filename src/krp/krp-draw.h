#ifndef KRP_DRAW_H
#define KRP_DRAW_H

/******************************************************************************
structures and functions to draw
******************************************************************************/

namespace krp
{
    namespace draw
    {
        typedef struct
        {
            float m_aafPos[4][2];    /* 0,0 -> top left. 1,1 -> bottom right. counter-clockwise */
            int m_iSprite;           /* 1 based index in SpriteName buffer. 0 = fill with m_ulColor */
            unsigned long m_ulColor; /* ABGR */
        } SPluginQuad_t;

        typedef struct
        {
            char m_szString[100];
            float m_afPos[2]; /* 0,0 -> top left. 1,1 -> bottom right */
            int m_iFont;      /* 1 based index in FontName buffer */
            float m_fSize;
            int m_iJustify;          /* 0 = left; 1 = center; 2 = right */
            unsigned long m_ulColor; /* ABGR */
        } SPluginString_t;
    }
}

#endif