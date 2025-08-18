#ifndef EXECUTOR_MONOTONIC_CLOCK_HPP
#define EXECUTOR_MONOTONIC_CLOCK_HPP

#include "clock.hpp"
#include <ctime>

namespace executor {

class MonotonicClock : public Clock
{
    
public: 

    MonotonicClock() = default;
    ~MonotonicClock() override = default;
    MonotonicClock(const MonotonicClock&) = default;
    MonotonicClock& operator=(const MonotonicClock&) = default;

    /**
     * @brief Retrieves the current monotonic time.
     * 
     * This method uses `clock_gettime` with `CLOCK_MONOTONIC` to fetch the current time.
     * 
     * @return The current monotonic time as a `Time` object.
     */
    Time time() const override;

    void sleep(Time const& delta) const override;

};

} // namespace Executor

#endif // EXECUTOR_MONOTONIC_CLOCK_HPP
