#ifndef PAIR_UTIL_HPP
#define PAIR_UTIL_HPP

#include <utility>

using std::pair;

// Helper function to make displacing blank space coordinates easier
template <typename T>
pair<T, T> operator+(const pair<T, T> &lhs, const pair<T, T> &rhs) {
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

#endif