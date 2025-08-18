#include "realtime_clock.hpp"
#include <cassert>
#include <cerrno>
#include <cstdio>

namespace executor {

Time RealTimeClock::time() const
{
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return Time(ts);
}

void RealTimeClock::sleep(Time const& delta) const
{
    timespec ts;
    ts.tv_nsec = delta.nanoseconds();
    ts.tv_sec = delta.seconds();
    timespec remain;
    
    int res = clock_nanosleep(CLOCK_REALTIME, 0, &ts, &remain);

    while(res != 0) {
        if(res == EINTR) {
            ts = remain;
            res = clock_nanosleep(CLOCK_REALTIME, 0, &ts, &remain);
        }
        else {
            perror("clock_nanosleep failed");
        } 
    }
}

} // namespace executor
