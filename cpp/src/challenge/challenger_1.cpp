#include "challenger_1.hpp"


/* Static Functions Declerations */

static unsigned long factorial_helper(unsigned long _n, unsigned long _acc);
static void swap_balls(Ball &_first, Ball &_second);

/* API Functions */

unsigned long fibg() {
  static unsigned long prev_prev = 1;
  static unsigned long prev = 1;

  unsigned long current = prev_prev;

  unsigned long sum = prev_prev + prev;

  prev_prev = prev;
  prev = sum;

  return current;
}

unsigned long factorial(unsigned long _n) { return factorial_helper(_n, 1); }

void sort_balls(std::vector<Ball> &_balls) {

  size_t _length = _balls.size();
  if (_length <= 1) {
    return;
  }

  size_t left = 0;
  size_t right = _length - 1;
  size_t i = 0;

  while (i <= right) {
    Ball &curr = _balls[i];
    switch (curr.color) {
      case White:
        swap_balls(_balls[left++], curr);
        i++;
        break;
      case Black:
        swap_balls(_balls[right--], curr);
        break;
      default:
        i++;
        break;
    }
  }
}

/* Static Functions */

static void swap_balls(Ball &_first, Ball &_second) {
  Ball temp = _first;
  _first = _second;
  _second = temp;
}

static unsigned long factorial_helper(unsigned long _n, unsigned long _acc) {
  if (_n <= 1) {
    return _acc;
  }
  return factorial_helper(_n - 1, _acc * _n);
}