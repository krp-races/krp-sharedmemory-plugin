#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <windows.h>

template <typename T>
class Memory
{
private:
    std::string BUFFER_NAME;
    DWORD BUFFER_SIZE_BYTES;
    HANDLE file = nullptr;
    T *data = nullptr;

public:
    Memory(std::string name);

    void create();
    void open();
    void close();
    T *get();
    T *write();
    T *reset();
};

#endif