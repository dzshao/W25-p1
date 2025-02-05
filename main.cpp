#include <iostream>
#include <vector>
#include <queue>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::priority_queue;

// Represents a given game state, the cost to reach the state, and the depth the node is at
struct node {
    vector<vector<int> > tiles;
    int cost;
    int depth;

    node(const vector<vector<int> > &tile, int cost, int depth) : tiles(tile), cost(cost), depth(depth) {};
    node(const vector<vector<int> > &tile, int cost) : tiles(tile), cost(cost), depth(0) {};
    node(const vector<vector<int> > &tile) : tiles(tile), cost(0), depth(0) {};

    // Comparison operator for compatiblity with priority_queue
    bool operator<(const node &rhs) const {
        return this->cost < rhs.cost;
    }
};

node general_search(node , int (*f) (const vector<vector<int> > &));
int misplaced_tile_heuristic(const vector<vector<int> > &);
int manhattan_heuristic(const vector<vector<int> > &);

int main() {
    vector<vector<int> > tiles = {{1,2,3}, {4,5,6}, {7,8,0}};

    cout << misplaced_tile_heuristic(tiles) << endl;

    node initial{tiles};


    
    return 0;
}

node general_search(node initialState, int (*f) (const vector<vector<int> > &heuristic_function)) {
    priority_queue<node> queue;
    queue.push(initialState);

    return node{{{0}}};
}

/* Takes in a 2D vector and returns the number of tiles that are in the incorrect position*/
int misplaced_tile_heuristic(const vector<vector<int> > &tiles) {
    int numMisplacedTiles = 0;
    int currVal = 1;
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            if (tiles.at(i).at(j) != currVal) {
                ++numMisplacedTiles;
            }
            ++currVal;
        }
    }

    // Tile in bottom corner should be 0, above for loop checks if it is equal to (numRows * numColumns) instead, so must correct for that
    if (tiles.at(numRows - 1).at(numColumns - 1) == 0) {
        --numMisplacedTiles;
    }
    return numMisplacedTiles;
}

int manhattan_heuristic(const vector<vector<int> > &tiles) {
    // TODO
    return 0;
}