#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>

enum LogLevel
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
private:
    static FILE *file;
    static const char * toLevelString(LogLevel level);

public:
    static void open(const char *filename);
    static void close();
    static void write(LogLevel level, const char *message);
    static void write(LogLevel level, int message);
    static void write(LogLevel level, float message);
};

#endif