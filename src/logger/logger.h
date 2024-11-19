#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>

class Logger
{
private:
    static FILE *file;

public:
    static void open(const char *filename);
    static void close();
    static void write(const char *message);
};

#endif