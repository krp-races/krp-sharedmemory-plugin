#include "lib.h"
#include "memory/memory.cpp"

EGameState prevGameState = EGameState::CLOSED;
Memory<SSharedMemory_t> mem(SHARED_MEMORY_NAME);

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

int SpectateVehicles(int _iNumVehicles, SPluginsSpectateVehicle_t *_pVehicleData, int _iCurSelection, int *_piSelect)
{
    return 0;
}

int SpectateCameras(int _iNumCameras, char *_pCameraData, int _iCurSelection, int *_piSelect)
{
    return 0;
}