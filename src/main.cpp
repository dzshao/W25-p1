#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <fstream>
#include <set>
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
using std::ostream;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::ifstream;
using std::set;

// Create 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance)
vector<pair<uint8_t, uint8_t> > solvedBoard;

// Maximum timeout value for the search (millisconds)
const int MAX_TIMEOUT = INT_MAX;

/* Takes in an initial node and a function to use as the heuristic function.
If no heuristic function is provided, it defaults to 0 every time. */
template <typename T>
node<T> general_search(node<T> , int (*) (const vector<vector<T> > &) = [](const vector<vector<T> > &) -> int {return 0;});

/* Takes in a 2D vector and returns the number of tiles that are in the incorrect position. */
template <typename T>
int misplaced_tile_heuristic(const vector<vector<T> > &);

/* Takes in a 2D vector and returns the total manhattan distance of the tiles. */
template <typename T>
int manhattan_heuristic(const vector<vector<T> > &);

/* Checks if a board is complete. */
template <typename T>
bool checkCompletedTile(const vector<vector<T> > &);

/* Returns the coordinates of the '0' space on a board. */
template <typename T>
pair<int, int> findBlank(const vector<vector<T> > &);

/* Returns the coordinates of any given tile on a board*/
template <typename T>
pair<int, int> findTile(const vector<vector<T> > &, int);

/* Print out board. */
template <typename T>
ostream& operator<<(ostream& out, const vector<vector<T> > &rhs);

int main() {
    vector<vector<int8_t> > tiles = {{0,1,6},
                                     {5,3,2},
                                     {4,7,8}};
    // vector<vector<int8_t> > tiles = {{13,10,9,7},
    //                                  {2,1,0,12},
    //                                  {15,11,14,3},
    //                                  {4,8,6,5}};
    
    // Create 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance)
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();

    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            solvedBoard.push_back({i, j});
        }
    }

    // cout << "Manhattan distance: " <<  manhattan_heuristic(tiles) << endl;
    cout << "Initial board: " << endl << tiles << endl
         << "Starting search..." << endl << endl;

    node initial{tiles};
    auto start = high_resolution_clock::now();
    node final = general_search(initial, manhattan_heuristic);
    auto stop = high_resolution_clock::now();

    cout << "Finished board: " << endl << final.tiles
         << "Solution depth: " << final.depth << endl;

    auto timeSpent = duration_cast<milliseconds>(stop - start);

    cout << "Time elapsed: " << timeSpent.count() / 1000.0 << " seconds" << endl;

    return 0;
}

template <typename T>
node<T> general_search(node<T> initialState, int (*heuristic_function) (const vector<vector<T> > &)) {
    // Create min-queue sorted based on cost of nodes
    priority_queue<node<T>, vector<node<T> >, greater<node<T> > > queue;
    queue.push(initialState);

    set<node<T>, bool(*)(const node<T>&, const node<T>&)> visitedStates(node<T>::setComparison);

    // Vectors to represent movement of blank tile (Right, Left, Up, Down)
    const vector<pair<int16_t, int16_t> > directionVectors = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

    node<T> currNode;

    int numNodesExpanded = 0;
    int maxQueueSize = 1;
    
    // Timer to check for timeout
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto timeSpent = duration_cast<milliseconds>(stop - start);

    while (!queue.empty() && timeSpent.count() < MAX_TIMEOUT) {
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
        pair<int, int> currCoord = initBlankCoord;
        for (pair<int, int> offset : directionVectors) {
            // Calculate new coordinate of blank tile
            currCoord = initBlankCoord + offset;

            // Check if movement is within bounds
            if (currCoord.first >= currNode.tiles.size() || currCoord.second >= currNode.tiles.at(currCoord.first).size() || currCoord.first < 0 || currCoord.second < 0) {
                continue;
            }

            node newNode(currNode.tiles, currNode.depth + 1, currNode.depth + 1);
            // Perform blank tile movement
            swap(newNode.tiles.at(initBlankCoord.first).at(initBlankCoord.second), newNode.tiles.at(currCoord.first).at(currCoord.second));
            if (visitedStates.count(newNode) != 0) {
                continue;
            }
            // Update cost of node using heuristic + cost to reach node (depth)
            newNode.cost += heuristic_function(newNode.tiles);

            queue.push(newNode);
        }
        ++numNodesExpanded;
        if (numNodesExpanded % 1000000 == 0) {
            cout << "Expanded: " << numNodesExpanded << " Max Queue Size: " << maxQueueSize << " Curr depth: " << currNode.depth << "Time Elapsed:" << duration_cast<milliseconds>(stop - start).count() / 1000.0 << endl << currNode.tiles;
        }
        visitedStates.insert(currNode);

        stop = high_resolution_clock::now();
        timeSpent = duration_cast<milliseconds>(stop - start);
    }

    if (!queue.empty()) {
        cout << "Search timeout" << endl;
    } else {
        cout << "Search failed" << endl;
    }
    cout << "Nodes expanded: " << numNodesExpanded << endl
         << "Max Queue Size: " << maxQueueSize << endl;

    // Failed to solve puzzle or timeout
    return node<T>({{0}}, -1, -1);
}

template <typename T>
int misplaced_tile_heuristic(const vector<vector<T> > &tiles)
{
    int numMisplacedTiles = 0;
    T expectedVal = 1;

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

template <typename T>
int manhattan_heuristic(const vector<vector<T> > &tiles) {
    int totalManhattanDistance = 0;
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();

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

template <typename T>
bool checkCompletedTile(const vector<vector<T> > &tiles) {
    // Must be solved if no tiles are misplaced
    if (misplaced_tile_heuristic(tiles) == 0) {
        return true;
    }
    return false;
}

template <typename T>
pair<int, int> findBlank(const vector<vector<T> > &tiles) {
    return findTile(tiles, 0);
}

template <typename T>
pair<int, int> findTile(const vector<vector<T> > &tiles, int num) {
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

template <typename T>
ostream& operator<<(ostream& out, const vector<vector<T> > &rhs) { 
    for (vector<T> row : rhs) {
        for (int tile : row) { 
            out << tile << " ";
        }
        out << endl;
    }
    return out;
}