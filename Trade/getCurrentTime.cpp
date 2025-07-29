#include "trade.h"

tm Trade::getCurrentTime()
{
    time_t rawTime;
    time(&rawTime);
    const struct tm* currentTime = localtime(&rawTime);
    return *currentTime;
}
