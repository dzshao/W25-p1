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

int main() {
    vector<vector<int> > tiles = {{1,3,6}, 
                                  {5,0,7}, 
                                  {4,8,2}};

    cout << misplaced_tile_heuristic(tiles) << endl;

    node initial{tiles};

    node final = general_search(initial, misplaced_tile_heuristic);
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

    while (!queue.empty()) {
        currNode = queue.top();
        queue.pop();
        if (checkCompletedTile(currNode.tiles)) {
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

    }
    return queue.top();
}

int misplaced_tile_heuristic(const vector<vector<int> > &tiles) {
    int numMisplacedTiles = 0;
    int currVal = 1;

    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    // Count number of tiles not in correct position
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

bool checkCompletedTile(const vector<vector<int> > &tiles) {
    if (misplaced_tile_heuristic(tiles) == 0) {
        return true;
    }
    return false;
}

pair<int, int> findBlank(const vector<vector<int> > &tiles) {
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            if (tiles.at(i).at(j) == 0) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}