#include "vector.hpp"


int main()
{

    std::vector<int> vec = {1,2,3,6,2,4};
    algo::print_vector(vec.begin(), vec.end());

    algo::sort_vector(vec);
    algo::print_vector(vec.begin(), vec.end());
}