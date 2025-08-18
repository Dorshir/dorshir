#ifndef EXECUTOR_TIME_HPP
#define EXECUTOR_TIME_HPP

#include <ctime>

namespace executor {

const long billion = 100000000L;
const long million = 1000000L;

class Time 
{

public:

    explicit Time(timespec const& ts);
    Time(time_t seconds, long nanoseconds);
    Time(size_t period_ms);
    
    Time(Time const& other) = default;
    Time& operator=(Time const& other) = default;
    ~Time() = default;

    bool operator<(Time const& other) const;
    bool operator>(Time const& other) const;
    Time operator-(Time const& other) const;
    Time operator+(Time const& other) const;
    Time& operator+=(Time const& other);
    // gb
    time_t seconds() const;
    long nanoseconds() const;

    /**
    * @brief Normalizes the `Time` object.
    * 
    * Ensures the nanoseconds value remains within the range [0, 1 billion).
    * Adjusts the seconds value accordingly.
    */
    void normalize();

private:

    timespec m_value;
};

} // namespace Executor

#endif // EXECUTOR_TIME_HPP
