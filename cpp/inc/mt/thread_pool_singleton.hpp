#ifndef THREAD_POOL_SINGLETON_HPP
#define THREAD_POOL_SINGLETON_HPP

namespace mt {

class TPSingle {

public:

    static ThreadPoolExecutor<>& create_pool(size_t capacity_of_tasks = 100);

private:

    TPSingle() = default;
    ~TPSingle() = default;
    TPSingle(TPSingle const&) = delete;
    TPSingle& operator=(TPSingle const&) = delete;
    TPSingle(TPSingle&&) noexcept = delete;
    TPSingle& operator=(TPSingle&&) noexcept = delete;
};

ThreadPoolExecutor<>& TPSingle::create_pool(size_t capacity_of_tasks)
{
    static ThreadPoolExecutor pool{capacity_of_tasks};
    return pool;
}

} // namespace mt

#endif // THREAD_POOL_SINGLETON_HPP