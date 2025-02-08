#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

using std::vector;

// Represents a given game state, the cost to reach the state, and the depth the node is at
template <typename T>
struct node {
    vector<vector<T> > tiles;
    int cost;
    int depth;

    node(const vector<vector<T> > &tile = {{0}}, int cost = 0, int depth = 0) : tiles(tile), cost(cost), depth(depth) {};

    // Comparison operators for compatiblity with priority_queue
    bool operator<(const node &rhs) const {
        return this->cost < rhs.cost;
    }
    bool operator>(const node &rhs) const {
        return this->cost > rhs.cost;
    }
    bool operator==(const node &rhs) const {
        if (this->tiles == rhs.tiles && this->depth >= rhs.depth) {
            return true;
        }
        return false;
    }

    // Used for compatibility with std::set
    static bool setComparison(const node &lhs, const node &rhs) {
        if (lhs.tiles != rhs.tiles) {
            return lhs.tiles < rhs.tiles;
        }
        // Returns false if tiles are the same
        return false;
    }
};

#endif