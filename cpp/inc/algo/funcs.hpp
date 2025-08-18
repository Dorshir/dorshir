#ifndef ALGO_FUNCS_IF_HPP
#define ALGO_FUNCS_IF_HPP

#include <cstddef> // for size_t
#include <vector>
#include <utility>
#include <algorithm> // for reverse

namespace algo {

template<typename T>
bool rot_matrix_90(std::vector<std::vector<T>>& matrix) 
{
    size_t n = matrix.size();
    if (n == 0 || matrix[0].size() != n) {
        return false;
    }

    // transpose diagonal
    for(size_t i = 0; i < n; ++i) {
        for(size_t j = i + 1 ; j < n; ++j) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }

    // reverse each row
    for (size_t i = 0; i < n; ++i) {
        std::reverse(matrix[i].begin(), matrix[i].end());
    }

    return true;
}

template <typename T>
bool rotate(std::vector<T>& arr, size_t n) 
{
    size_t size = arr.size();
    if (size == 0 || n == 0) {
        return false;
    }
    n = n % size;

    // reverse the entire array
    std::reverse(arr.begin(), arr.end());

    // reverse the first n
    std::reverse(arr.begin(), arr.begin() + n);

    // reverse the remainings
    std::reverse(arr.begin() + n, arr.end());
}

} // namespace algo





#endif // ALGO_FUNCS_IF_HPP