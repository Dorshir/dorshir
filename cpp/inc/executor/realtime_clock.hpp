#ifndef EXECUTOR_REALTIME_CLOCK_HPP
#define EXECUTOR_REALTIME_CLOCK_HPP

#include "clock.hpp"
#include <ctime>

namespace executor {

class RealTimeClock : public Clock
{
public:

    RealTimeClock() = default;
    ~RealTimeClock() override = default;
    RealTimeClock(const RealTimeClock&) = default;
    RealTimeClock& operator=(const RealTimeClock&) = default;

    /**
    * @brief Retrieves the current realtime time.
    * 
    * This method uses `clock_gettime` with `CLOCK_REALTIME` to fetch the current time.
    * 
    * @return The current realtime time as a `Time` object.
    */
    Time time() const override;

    void sleep(Time const& delta) const override;
};

} // namespace Executor

#endif // EXECUTOR_REALTIME_CLOCK_HPP
