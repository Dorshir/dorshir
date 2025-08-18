#ifndef EXECUTOR_CLOCK_HPP
#define EXECUTOR_CLOCK_HPP

#include "time.hpp"

namespace executor {

class Clock 
{

public:

    explicit Clock() = default;
    virtual ~Clock() = default;

    /**
     * @brief Retrieves the current time.
     * 
     * @return The current time as a `Time` object.
     */
    virtual Time time() const = 0;

    virtual void sleep(Time const& delta) const = 0;
};

} // namespace executor

#endif // EXECUTOR_CLOCK_HPP
