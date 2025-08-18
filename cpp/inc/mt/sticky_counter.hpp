#ifndef MT_STICKY_COUNTER_HPP
#define MT_STICKY_COUNTER_HPP

#include <atomic>

namespace mt {

class StickyCounter {

public:

    explicit StickyCounter(int initial_value);
    ~StickyCounter() = default;

    StickyCounter(const StickyCounter&) = delete;
    StickyCounter& operator=(const StickyCounter&) = delete;
    StickyCounter(StickyCounter&&) noexcept = delete;
    StickyCounter& operator=(StickyCounter&&) noexcept = delete;

public:

    int read() const;
    void decrement();
    void increment();

    void reset(int value);

private:

    std::atomic<int> m_count;
};

StickyCounter::StickyCounter(int initial_value)
: m_count{initial_value}
{}


int StickyCounter::read() const
{
    return m_count.load();
}


void StickyCounter::decrement()
{
    int expected = m_count.load();
    while (expected > 0 && !m_count.compare_exchange_weak(expected, expected - 1))
    {
        expected = m_count.load();
    }
}


void StickyCounter::increment()
{
    int expected = m_count.load();
    while (expected > 0 && !m_count.compare_exchange_weak(expected, expected + 1))
    {
        expected = m_count.load();
    }
}


void StickyCounter::reset(int value)
{
    m_count.store(value);
}


}  // namespace mt

#endif  // MT_STICKY_COUNTER_HPP