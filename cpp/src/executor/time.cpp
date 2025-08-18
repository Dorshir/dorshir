#include "time.hpp"
#include <ctime>
#include <cstddef> // for size_t

namespace executor {


Time::Time(timespec const& ts)
: m_value{ts}
{
    normalize();
}

Time::Time(time_t seconds, time_t nanoseconds)
: m_value{seconds, nanoseconds}
{
    normalize();
}

Time::Time(size_t period_ms)
: m_value{static_cast<time_t>(period_ms / 1000), static_cast<long>((period_ms % 1000) * executor::million)}
{}

bool Time::operator<(Time const& other) const
{
    if (m_value.tv_sec != other.m_value.tv_sec) {
        return m_value.tv_sec < other.m_value.tv_sec;
    }
    return m_value.tv_nsec < other.m_value.tv_nsec;
}

bool Time::operator>(Time const& other) const
{
    return other < *this;
}

Time Time::operator-(Time const& other) const
{
    timespec result;
    result.tv_sec = m_value.tv_sec - other.m_value.tv_sec;//
    result.tv_nsec = m_value.tv_nsec - other.m_value.tv_nsec;

    if (result.tv_nsec < 0) {
        result.tv_sec -= 1;
        result.tv_nsec += billion;
    }
    return Time{result};
}

Time Time::operator+(Time const& other) const
{
    timespec result{};
    result.tv_sec = m_value.tv_sec + other.m_value.tv_sec;
    result.tv_nsec = m_value.tv_nsec + other.m_value.tv_nsec;

    Time sum(result);
    sum.normalize();
    return sum;
}

Time& Time::operator+=(Time const& other)
{
    m_value.tv_sec += other.m_value.tv_sec;
    m_value.tv_nsec += other.m_value.tv_nsec;

    normalize();
    return *this;
}

time_t Time::seconds() const
{
    return m_value.tv_sec;
}

time_t Time::nanoseconds() const
{
    return m_value.tv_nsec;
}

void Time::normalize()
{
    if (m_value.tv_nsec >= billion) {
        m_value.tv_sec += m_value.tv_nsec / billion;
        m_value.tv_nsec %= billion;
    } else if (m_value.tv_nsec < 0) {
        time_t seconds_adjust = (m_value.tv_nsec / billion) - 1;
        m_value.tv_sec += seconds_adjust;
        m_value.tv_nsec -= seconds_adjust * billion;
    }
}


} // namespace Executor