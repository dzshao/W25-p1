#ifndef PAIR_UTIL_HPP
#define PAIR_UTIL_HPP

#include <utility>

using std::pair;

// Helper function to make displacing blank space coordinates easier
template <typename T>
pair<T, T> operator+(const pair<T, T> &lhs, const pair<T, T> &rhs) {
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

template <typename T>
pair<T, T> operator-(const pair<T, T> &lhs, const pair<T, T> &rhs) {
    return {lhs.first - rhs.first, lhs.second - rhs.second};
}

// Returns the manhattan distance between two coordinates
template <typename T>
int manhattan_distance(const pair<T, T> &lhs, const pair<T, T> &rhs) {
    return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
}

#endif