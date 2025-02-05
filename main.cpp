#include <iostream>
#include <vector>
#include <queue>
#include "node.hpp"
#include "pair_util.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::priority_queue;
using std::pair;
using std::swap;
using std::greater;
using std::optional;
using std::nullopt;

/* Takes in an initial node and a pointer to a function to use as the heuristic function.
If no heuristic function is provided, it defaults to 0 every time. */
node general_search(node , int (*) (const vector<vector<int> > &) = [](const vector<vector<int> > &) -> int {return 0;});

/* Takes in a 2D vector and returns the number of tiles that are in the incorrect position. */
int misplaced_tile_heuristic(const vector<vector<int> > &);

/* Takes in a 2D vector and returns the total manhattan distance of the tiles. */
int manhattan_heuristic(const vector<vector<int> > &);

/* Checks if a board is complete.  */
bool checkCompletedTile(const vector<vector<int> > &);

/* Returns the coordinates of the '0' space on a board*/
pair<int, int> findBlank(const vector<vector<int> > &);

/* Returns the coordinates of any given tile on a board*/
pair<int, int> findTile(const vector<vector<int> > &, int);

int main() {
    vector<vector<int> > tiles = {{8,6,7}, 
                                  {2,5,4}, 
                                  {3,0,1}};

    cout << "Manhattan distance: " <<  manhattan_heuristic(tiles) << endl;
    node initial{tiles};

    node final = general_search(initial, manhattan_heuristic);
    for (vector<int> row : final.tiles) {
        for (int tile : row) { 
            cout << tile << " ";
        }
        cout << endl;
    }
    cout << "Final depth: " << final.depth << endl;

    return 0;
}

node general_search(node initialState, int (*heuristic_function) (const vector<vector<int> > &)) {
    priority_queue <node, vector<node>, greater<node>> queue;
    queue.push(initialState);

    // Vectors to represent movement of blank tile (Right, Left, Up, Down)
    vector<pair<int, int> > directionVectors = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

    node currNode;

    int numNodesExpanded = 0;
    int maxQueueSize = 1;
    while (!queue.empty()) {
        if (queue.size() > maxQueueSize) {
            maxQueueSize = queue.size();
        }
        currNode = queue.top();
        queue.pop();
        if (checkCompletedTile(currNode.tiles)) {
            cout << "Nodes expanded: " << numNodesExpanded << endl
                 << "Max Queue Size: " << maxQueueSize << endl;
            return currNode;
        }

        pair<int, int> initBlankCoord = findBlank(currNode.tiles);
        for (pair<int, int> offset : directionVectors) {
            pair<int, int> currCoord = initBlankCoord + offset;

            // Check if movement is within bounds
            if (currCoord.first >= currNode.tiles.size() || currCoord.second >= currNode.tiles.at(currCoord.first).size()) {
                continue;
            }

            node newNode{currNode.tiles, 0, currNode.depth + 1};
            // Perform blank tile movement
            swap(newNode.tiles.at(initBlankCoord.first).at(initBlankCoord.second), newNode.tiles.at(currCoord.first).at(currCoord.second));
            // Update cost of node using heuristic + cost to reach node (depth)
            newNode.cost = newNode.depth + heuristic_function(newNode.tiles);

            queue.push(newNode);
        }
        ++numNodesExpanded;
    }
    return {};
}

int misplaced_tile_heuristic(const vector<vector<int> > &tiles) {
    int numMisplacedTiles = 0;
    int expectedVal = 1;

    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    int currVal = tiles.at(0).at(0);
    // Count number of tiles not in correct position
    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            currVal = tiles.at(i).at(j);
            
            // Skip over blank tile
            if (currVal == 0) {
                ++expectedVal;
                continue;
            }
            if (currVal != expectedVal) {
                ++numMisplacedTiles;
            }
            ++expectedVal;
        }
    }
    return numMisplacedTiles;
}

int manhattan_heuristic(const vector<vector<int> > &tiles) {
    vector<pair<int, int> > solvedBoard;
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();

    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            solvedBoard.push_back({i, j});
        }
    }

    int totalManhattanDistance = 0;
    numRows = tiles.size();
    numColumns = tiles.at(0).size();

    int currVal = tiles.at(0).at(0);
    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            currVal = tiles.at(i).at(j);
            // Skip over blank tile
            if (currVal == 0) {
                continue;
            }
            totalManhattanDistance += abs(i - solvedBoard.at(currVal - 1).first) + abs(j - solvedBoard.at(currVal - 1).second);
        }
    }

    return totalManhattanDistance;
}

bool checkCompletedTile(const vector<vector<int> > &tiles) {
    if (misplaced_tile_heuristic(tiles) == 0) {
        return true;
    }
    return false;
}

pair<int, int> findBlank(const vector<vector<int> > &tiles) {
    return findTile(tiles, 0);
}

pair<int, int> findTile(const vector<vector<int> > &tiles, int num) {
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            if (tiles.at(i).at(j) == num) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}