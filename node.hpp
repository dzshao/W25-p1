#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

using std::vector;

// Represents a given game state, the cost to reach the state, and the depth the node is at
struct node {
    vector<vector<int> > tiles;
    int cost;
    int depth;

    node(const vector<vector<int> > &tile = {{0}}, int cost = 0, int depth = 0) : tiles(tile), cost(cost), depth(depth) {};

    // Comparison operators for compatiblity with priority_queue
    bool operator<(const node &rhs) const {
        return this->cost < rhs.cost;
    }
    bool operator>(const node &rhs) const {
        return this->cost > rhs.cost;
    }
    bool operator==(const node &rhs) const {
        return this->cost == rhs.cost;
    }
};

#endif