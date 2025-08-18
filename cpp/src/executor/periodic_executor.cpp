#include "periodic_executor.hpp"
#include <cstddef> // for size_t
#include <cassert> 

namespace executor {


PeriodicExecutor::PeriodicExecutor(std::string const &name, Clock &clock)
: m_name{name}
, m_clock{clock}
, m_pause_flag{false}
, m_tasks{}
{}


bool PeriodicExecutor::add(RunnableTask *task, size_t period_ms)
{
    if (task == nullptr) {
        return false;
    }

    Time period(period_ms);

    // Push into the priority queue
    ScheduledTask st(task, period, period);
    m_tasks.emplace(st);

    return true;
}


size_t PeriodicExecutor::run()
{
    m_pause_flag = false;
    size_t execute_cycles = 0;
    // need to add to a vector, then when calling run, schedule to p queue
    while (!m_pause_flag && !m_tasks.empty()) {

        ScheduledTask top_task = m_tasks.top();
        m_tasks.pop();

        Time now = m_clock.time();
        if (top_task.next_execution_time > now){
            Time delta = top_task.next_execution_time - now;
            m_clock.sleep(delta);
        }


        int status = top_task.task->execute();

        if (status == 0) {
            // Re-schedule
            Time new_next = m_clock.time() + top_task.period;
            top_task.next_execution_time = new_next;
            m_tasks.push(top_task);
        }
        ++execute_cycles;
    }

    return execute_cycles;
}

size_t PeriodicExecutor::pause()
{
    m_pause_flag = true;
    return m_tasks.size();
}


/* Inner Struct */

PeriodicExecutor::ScheduledTask::ScheduledTask(RunnableTask *task, Time const& period, Time const& next_time)
: task{task}
, period{period}
, next_execution_time{next_time}
{
    assert(task);
}


} // namespace Executor
