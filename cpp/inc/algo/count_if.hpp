#ifndef ALGO_COUNT_IF_HPP
#define ALGO_COUNT_IF_HPP

#include <cstddef> // for size_t

namespace algo {

template<typename Iterator, typename Predicate>
size_t count_if(Iterator begin, Iterator end, Predicate pred) {

    size_t trues_count = 0;
    for (auto it = begin; it != end; ++it) {
        if(pred(*it)) {
            ++trues_count;
        }
    }
    return trues_count;   
}


template<typename Predicate_1, typename Predicate_2>
auto and_combinator(Predicate_1 pred_1, Predicate_2 pred_2) {
    
    return [pred_1, pred_2](auto const& element) {
        return pred_1(element) && pred_2(element);
    };
}

} // namespace algo





#endif // ALGO_COUNT_IF_HPP