#ifndef MT_PAR_ALGORITHMS_HPP
#define MT_PAR_ALGORITHMS_HPP

#include "thread_pool_executor.hpp"
#include "thread_pool_singleton.hpp"

#include <vector>
#include <future>
#include <algorithm>
#include <iterator>
#include <cstddef>

namespace mt {


template<typename T, typename Iterator, typename ReduceFunc, typename MapFunc>
T par_map_reduce(Iterator begin, Iterator end, T init_value, ReduceFunc reduce_func, MapFunc map_func) 
{
    auto& pool = mt::TPSingle::create_pool();

    size_t worker_count = pool.worker_size();

    size_t total_size = std::distance(begin, end);
    size_t chunk_size = (total_size + worker_count - 1) / worker_count;

    std::vector<std::future<T>> futures;
    futures.reserve(worker_count);

    for (size_t w = 0; w < worker_count; ++w) {
        auto chunk_begin = std::next(begin, static_cast<long>(w * chunk_size));
        if (chunk_begin >= end) {
            break;
        }

        auto chunk_end = (w == worker_count - 1) ? end : std::next(chunk_begin, static_cast<long>(chunk_size));
        if (chunk_end > end) {0
            chunk_end = end;
        }

        auto promise = std::make_shared<std::promise<T>>();
        futures.push_back(promise->get_future());

        pool.submit([chunk_begin, chunk_end, &reduce_func, &map_func, init_value, promise = std::move(promise)]() {
            T partial = init_value;
            auto it = chunk_begin;
            while (it != chunk_end) {
                partial = reduce_func(partial, map_func(*it));
                ++it;
            }
            promise->set_value(partial);
        });
    }

    T final_result = init_value;
    for (auto& f : futures) {
        final_result = reduce_func(final_result, f.get());
    }

    return final_result;
}


template<typename T, typename Container, typename MaxFunc>
T par_max(Container const& container, T init_value, MaxFunc max_func) 
{
    ThreadPoolExecutor pool{};
    size_t worker_count = pool.worker_size();
    
    auto begin = container.begin();
    auto end = container.end();

    size_t total_size = std::distance(begin, end);
    size_t chunk_size = (total_size + worker_count - 1) / worker_count; 

    std::vector<std::future<T>> futures;
    futures.reserve(worker_count);

    for (size_t w = 0; w < worker_count; ++w) {
        auto chunk_begin = std::next(begin, static_cast<long>(w * chunk_size));
        if (chunk_begin >= end) {
            break;
        }

        auto chunk_end = (w == worker_count - 1) ? end : std::next(chunk_begin, static_cast<long>(chunk_size));
        if (chunk_end > end) {
            chunk_end = end;
        }

        auto promise = std::make_shared<std::promise<T>>();
        futures.push_back(promise->get_future());

        pool.submit([chunk_begin, chunk_end, &max_func, promise]() {
            auto it = chunk_begin;
            T partial = *it;
            ++it;

            while (it != chunk_end) {
                partial = max_func(partial, *it);
                ++it;
            }
            
            promise->set_value(partial);
        });
    }

    T final_result = init_value;
    for (auto& f : futures) {
        final_result = max_func(final_result, f.get());
    }

    return final_result;
}
    

} // namespace mt

#endif // MT_PAR_ALGORITHMS_HPP
