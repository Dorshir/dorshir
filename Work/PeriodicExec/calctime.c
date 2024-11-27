#define _POSIX_C_SOURCE 199309L

#include "calctime.h"
#include <time.h>

size_t GetCurrentTime_ms(clockid_t clk_id)
{
    struct timespec ts;
    if (clock_gettime(clk_id, &ts) != 0)
    {
        return 0;
    }
    return (size_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}