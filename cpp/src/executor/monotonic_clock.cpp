#include "monotonic_clock.hpp"
#include <cassert>
#include <cerrno>
#include <cstdio>

namespace executor {

Time MonotonicClock::time() const
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return Time{ts};
}

void MonotonicClock::sleep(Time const& delta) const
{
    timespec ts;
    ts.tv_nsec = delta.nanoseconds();
    ts.tv_sec = delta.seconds();

    timespec remain;
    
    int res = clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, &remain);

    while(res != 0) {
        if(res == EINTR) {
            ts = remain;
            res = clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, &remain);
        }
        else {
            perror("clock_nanosleep failed");
        } 
    }
}

} // namespace executor
