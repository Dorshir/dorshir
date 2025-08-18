#ifndef ALGO_VECTOR_HPP
#define ALGO_VECTOR_HPP

#include <vector>
#include <iostream>


namespace algo {


template <typename ForwardIter>
// ForwardIter: const_iterator/iterator
void print_vector(ForwardIter begin, ForwardIter end, char const* seperator = ", ", std::ostream& os = std::cout)
{
    os << "[ ";
    while (begin != end) {
        os << *begin++;
        if (begin != end) {
            os << seperator;
        }
    }
    os << " ]" << std::endl;
}


template <typename Container>
void sort_container(Container& container)
{
    typename Container::iterator end = container.end();
    typename Container::iterator itr = container.begin();

    while (itr != end){

        typename Container::iterator min_itr = itr;
        typename Container::iterator j = itr;
        ++j;

        while (j != end){

            if (*j < *min_itr) {
                min_itr = j;
            }

            ++j;
        }
        if (min_itr != itr) {
            std::swap(*itr, *min_itr);
        }
        ++itr;
    }
}

template <typename T>
void shuffel_vector(std::vector<T>& vec)
{

}

}






#endif // ALGO_VECTOR_HPP