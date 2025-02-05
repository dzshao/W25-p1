#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

using std::vector;

// Represents a given game state, the cost to reach the state, and the depth the node is at
struct node {
    vector<vector<int> > tiles;
    int cost;
    int depth;

    node(const vector<vector<int> > &tile, int cost, int depth) : tiles(tile), cost(cost), depth(depth) {};
    node(const vector<vector<int> > &tile, int cost) : tiles(tile), cost(cost), depth(0) {};
    node(const vector<vector<int> > &tile) : tiles(tile), cost(0), depth(0) {};
    node() : tiles({{0}}), cost(0), depth(0) {};

    // Comparison operator for compatiblity with priority_queue
    bool operator<(const node &rhs) const {
        return this->cost < rhs.cost;
    }
    bool operator>(const node &rhs) const {
        return this->cost > rhs.cost;
    }
};

#endif