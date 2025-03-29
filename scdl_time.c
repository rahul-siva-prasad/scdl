#include "scdl_time.h"

char* getTime(void) 
{
    static char time_buffer[25];
    time_t rawtime;
    struct tm timeinfo;

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    size_t bufferEnd = strftime(time_buffer, (size_t)sizeof(time_buffer), "%d-%m-%y %I:%M:%S %p", &timeinfo);

    time_buffer[bufferEnd] = '\0';
    return time_buffer;
}