#include "logger.h"

FILE *Logger::file = nullptr;

void Logger::open(const char *filename)
{
    if (file != nullptr)
        Logger::close();
    fopen_s(&Logger::file, filename, "a");
}

void Logger::close()
{
    if (file == nullptr)
        return;
    fclose(file);
    file = nullptr;
}

void Logger::write(const char *message)
{
    Logger::open("krp-sharedmemory-plugin.log");
    fprintf(file, "%s\n", message);
    Logger::close();
}
