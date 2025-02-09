#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <vector>
#include <iostream>
#include <chrono>
#include <queue>
#include <set>
#include "./node.hpp"
#include "./board_util.hpp"
#include "./pair_util.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::priority_queue;
using std::set;
using std::swap;
using std::greater;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

/* 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance) */
vector<pair<uint8_t, uint8_t> > solvedBoard = {};

/* Maximum timeout value for the search (millisconds). */
const int MAX_TIMEOUT = 900000;

/* Enables printing out the current node being expanded. */
const bool ENABLE_TRACEBACK = false;

/* Solves the given board. The integer indiciates the search function to use (1 = Uniform Cost Search, 2 = Misplaced Tile, 3 = Manhattan Distance).
Prints out the number of nodes expanded, max queue size, and time elapsed once completed. */
template <typename T>
void solveBoard(const vector<vector<T> > &, int);

/* Takes in an initial node and a function to use as the heuristic function.
If no heuristic function is provided, it defaults to 0 every time. 
Returns the node of the solved state. */
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

/* Create 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance). */
template <typename T>
void initializeSolvedBoard(const vector<vector<T> > &);

/* Solves the given board. The integer indiciates the search function to use (1 = Uniform Cost Search, 2 = Misplaced Tile, 3 = Manhattan Distance).
Prints out the number of nodes expanded, max queue size, and time elapsed once completed. */
template <typename T>
void solveBoard(const vector<vector<T> > &tiles, int searchNum) {
    // Create 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance)
    initializeSolvedBoard(tiles);

    cout << "Initial board: " << endl << tiles << endl
         << "Starting search..." << endl << endl;

    node initial{tiles};
    node<T> final; 
    auto start = high_resolution_clock::now();
    if (searchNum == 1) {
        cout << "Search used: Uniform Cost Search" << endl;
        final = general_search(initial);
    } else if (searchNum == 2) {
        cout << "Search used: Misplaced Tile" << endl;
        final = general_search(initial, misplaced_tile_heuristic);
    } else if (searchNum == 3) {
        cout << "Search used: Manhattan distance" << endl;
        final = general_search(initial, manhattan_heuristic);
    } else {
        cerr << "Error: Invalid search number provided (expected [1,2,3], received \'" << searchNum << "\')" << endl;
        throw std::exception();
    }

    auto stop = high_resolution_clock::now();
    auto timeSpent = duration_cast<milliseconds>(stop - start);

    cout << "Solution depth: " << final.depth << endl
         << "Time elapsed: " << timeSpent.count() / 1000.0 << " seconds" << endl;
    
    return;
}

/* Takes in an initial node and a function to use as the heuristic function.
If no heuristic function is provided, it defaults to 0 every time. 
Returns the node of the solved state. */
template <typename T>
node<T> general_search(node<T> initialState, int (*heuristic_function) (const vector<vector<T> > &)) {
    // Create min-queue sorted based on cost of nodes
    priority_queue<node<T>, vector<node<T> >, greater<node<T> > > queue;
    initialState.cost = heuristic_function(initialState.tiles);
    queue.push(initialState);

    set<node<T>, bool(*)(const node<T>&, const node<T>&)> visitedStates(node<T>::setComparison);

    // Vectors to represent movement of blank tile (Right, Left, Up, Down)
    const vector<pair<int16_t, int16_t> > directionVectors = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

    node<T> currNode;

    int numNodesExpanded = 0;
    unsigned long maxQueueSize = 1;
    
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
            cout << "Goal state found" << endl
                 << "Nodes expanded: " << numNodesExpanded << endl
                 << "Max Queue Size: " << maxQueueSize << endl;
            return currNode;
        }
                
        if (ENABLE_TRACEBACK) {
            cout << "Best node to expand with g(n) = " << currNode.depth << " and h(n) = " << (currNode.cost - currNode.depth) << endl
                << currNode.tiles << endl;
        }

        pair<int, int> initBlankCoord = findBlank(currNode.tiles);
        pair<int, int> currCoord = initBlankCoord;

        // Try all possible moves
        for (pair<int, int> offset : directionVectors) {
            // Calculate new coordinate of blank tile
            currCoord = initBlankCoord + offset;

            // Check if movement is within bounds
            if (currCoord.second < 0 || currCoord.first < 0 || currCoord.first >= static_cast<int>(currNode.tiles.size()) || currCoord.second >= static_cast<int>(currNode.tiles.at(currCoord.first).size())) {
                continue;
            }

            node newNode(currNode.tiles, currNode.depth + 1, currNode.depth + 1);
            // Perform blank tile movement
            swap(newNode.tiles.at(initBlankCoord.first).at(initBlankCoord.second), newNode.tiles.at(currCoord.first).at(currCoord.second));

            // Skip if node was already visited
            if (visitedStates.count(newNode) != 0) {
                continue;
            }
            
            // Update cost of node using heuristic + cost to reach node (depth)
            newNode.cost += heuristic_function(newNode.tiles);

            queue.push(newNode);
        }
        ++numNodesExpanded;
        // if (numNodesExpanded % 1000000 == 0) {
        //     cout << "Expanded: " << numNodesExpanded << " Max Queue Size: " << maxQueueSize << " Curr depth: " << currNode.depth << " Time Elapsed: " << duration_cast<milliseconds>(stop - start).count() / 1000.0 << " seconds" << endl << currNode.tiles;
        // }
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

/* Takes in a 2D vector and returns the number of tiles that are in the incorrect position. */
template <typename T>
int misplaced_tile_heuristic(const vector<vector<T> > &tiles) {
    int numMisplacedTiles = 0;
    T expectedVal = 1;

    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    T currVal = tiles.at(0).at(0);
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

/* Takes in a 2D vector and returns the total manhattan distance of the tiles. */
template <typename T>
int manhattan_heuristic(const vector<vector<T> > &tiles) {
    // Check if solvedBoard vector is initialized
    if (solvedBoard.empty()) {
        initializeSolvedBoard(tiles);
    }
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
            // Calculate manhattan distance by finding difference between actual x and y coordinate and the expected coordinate
            totalManhattanDistance += abs(i - solvedBoard.at(currVal - 1).first) + abs(j - solvedBoard.at(currVal - 1).second);
        }
    }

    return totalManhattanDistance;
}

/* Checks if a board is complete. */
template <typename T>
bool checkCompletedTile(const vector<vector<T> > &tiles) {
    // Must be solved if no tiles are misplaced
    if (misplaced_tile_heuristic(tiles) == 0) {
        return true;
    }
    return false;
}

/* Create 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance). */
template <typename T>
void initializeSolvedBoard(const vector<vector<T> > &tiles) {
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    
    solvedBoard = {};

    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            solvedBoard.push_back({i, j});
        }
    }
}

#endif