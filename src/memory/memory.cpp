#include "memory.h"

template <typename T>
Memory<T>::Memory(std::string name)
{
    BUFFER_NAME = name;
    BUFFER_SIZE_BYTES = sizeof(T);
}

template <typename T>
void Memory<T>::create()
{
    file = CreateFileMapping(
        INVALID_HANDLE_VALUE, // use paging file
        NULL,                 // default security
        PAGE_READWRITE,       // read/write access
        0,                    // maximum object size (high-order DWORD)
        BUFFER_SIZE_BYTES,    // maximum object size (low-order DWORD)
        BUFFER_NAME.c_str()); // name of mapping object

    if (file == nullptr)
        throw std::runtime_error("Couldn't create file mapping (" + BUFFER_NAME + ").");
}

template <typename T>
void Memory<T>::open()
{
    file = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,  // read/write access
        FALSE,                // do not inherit the name
        BUFFER_NAME.c_str()); // name of mapping object

    if (file == nullptr)
        throw std::runtime_error("Couldn't open file mapping (" + BUFFER_NAME + ").");
}

template <typename T>
void Memory<T>::close()
{
    if (data != nullptr)
        UnmapViewOfFile(data);

    if (file != nullptr)
        CloseHandle(file);

    data = nullptr;
    file = nullptr;
}

template <typename T>
T *Memory<T>::get()
{
    if (file == nullptr)
        throw std::runtime_error("No file mapping (" + BUFFER_NAME + ") opened.");

    if (data == nullptr)
        data = (T *)MapViewOfFile(file, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE_BYTES);

    return data;
}

template <typename T>
T *Memory<T>::write()
{
    if (file == nullptr)
        throw std::runtime_error("No file mapping (" + BUFFER_NAME + ") opened.");

    if (data == nullptr)
        data = (T *)MapViewOfFile(file, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE_BYTES);

    if (!FlushViewOfFile(data, BUFFER_SIZE_BYTES))
        throw std::runtime_error("Couldn't flush view of file (" + BUFFER_NAME + ").");

    return data;
}