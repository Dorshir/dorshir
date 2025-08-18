#ifndef CHALLENGER_1_HPP
#define CHALLENGER_1_HPP
#include <vector>


enum Color {
    Red,  
    Black, 
    White  
};

struct Ball {
    Color color;     
    int radius;      
    bool is_solid;   
};

/**
 * @brief Generates the next Fibonacci number in sequence.
 *
 * This function maintains an internal state and returns the next number 
 * in the Fibonacci sequence on each call. It starts with 1 and 1.
 *
 * @return The next Fibonacci number in the sequence.
 */
unsigned long fibg();

/**
 * @brief Calculates the factorial of a given number.
 *
 * This function computes the factorial of a non-negative integer using recursion.
 *
 * @param _n The number to calculate the factorial for.
 * @return The factorial of the number. If `_n` is 0, returns 1.
 */
unsigned long factorial(unsigned long _n);

/**
 * @brief Sorts a vector of balls by their color.
 *
 * This function sorts balls in the order: White, Red, and Black. 
 * The sorting algorithm uses the Dutch National Flag problem solution.
 *
 * @param _balls A reference to the vector of balls to be sorted.
 */
void sort_balls(std::vector<Ball> &_balls);


#endif /* CHALLENGER_1_HPP */