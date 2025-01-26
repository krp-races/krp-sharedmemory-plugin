#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>

std::string SHARED_MEMORY_NAME = "$krp$";

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

typedef struct
{
    int sequenceNumber;
    EGameState gameState;
} SSharedMemory_t;

#endif