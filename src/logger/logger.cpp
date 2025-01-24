#include "logger.h"

FILE *Logger::file = nullptr;

const char *Logger::toLevelString(LogLevel level)
{
    switch (level)
    {
    case DEBUG:
        return "DEBUG";
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

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

void Logger::write(LogLevel level, const char *message)
{
    Logger::open("krp-sharedmemory-plugin.log");
    fprintf(file, "%s - %s\n", toLevelString(level), message);
    Logger::close();
}

void Logger::write(LogLevel level, int message)
{
    Logger::open("krp-sharedmemory-plugin.log");
    fprintf(file, "%s - %d\n", toLevelString(level), message);
    Logger::close();
}

void Logger::write(LogLevel level, float message)
{
    Logger::open("krp-sharedmemory-plugin.log");
    fprintf(file, "%s - %f\n", toLevelString(level), message);
    Logger::close();
}
