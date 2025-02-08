#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <fstream>
#include <sstream>
#include <set>
#include "node.hpp"
#include "pair_util.hpp"

using std::cout;
using std::cerr;
using std::getline;
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
using std::stringstream;
using std::set;
using std::string;

/* Create 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance) */
vector<pair<uint8_t, uint8_t> > solvedBoard;

/* Maximum timeout value for the search (millisconds) */
const int MAX_TIMEOUT = INT_MAX;

/* Solves the given board. The integer indiciates the search function to use (1 = Uniform Cost Search, 2 = Misplaced Tile, 3 = Manhattan Distance).
Prints out the number of nodes expanded, max queue size, and time elapsed once completed. */
template <typename T>
void solveBoard(const vector<vector<T> >, int);

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

/* Returns the coordinates of the '0' space on a board. */
template <typename T>
pair<int, int> findBlank(const vector<vector<T> > &);

/* Returns the coordinates of any given tile on a board*/
template <typename T>
pair<int, int> findTile(const vector<vector<T> > &, int);

/* Print out board. */
template <typename T>
ostream& operator<<(ostream& out, const vector<vector<T> > &rhs);

/* Parses a single test case from a file. 
Returns a pair<int, vector<..> > where the int represents the search to use and the vector is the initial board. */
template <typename T>
pair<int, vector<vector<T> > > parseTestCase(ifstream&);

int main(int argc, char* argv[]) {
    if (argc > 2) {
        cerr << "Usage: ./program [input_test.txt]" << endl;
        return -1;
    }

    // Default case if no testcase is provided
    if (argc == 1) {
        cout << "No file provided, using default test case." << endl
             << "Select which search function to use, Uniform Cost Search (1), Misplaced Tile (2), Manhattan Distance (3): ";
        int searchNum = 0;
        cin >> searchNum;
        cout << endl;

        vector<vector<int8_t> > tiles= {{0,1,6},
                                        {5,3,2},
                                        {4,7,8}};

        solveBoard(tiles, searchNum);
    } else {
        ifstream fin;
        fin.open(argv[1]);
        if (!fin.is_open()) {
            cerr << "Error: Failed to open file \'" << argv[1] << "\'" << endl;
            return -1;
        }

        // Keep looping until end of file
        while (fin) {
            pair<int, vector<vector<int16_t> > > test = parseTestCase<int16_t>(fin);
            solveBoard(test.second, test.first);
        }
    }

    return 0;
}

template <typename T>
void solveBoard(const vector<vector<T> > tiles, int searchNum) {
        // Create 0-indexed list of correct coordinates for the respective tile number (used for manhattan distance)
    int numRows = tiles.size();
    int numColumns = tiles.at(0).size();
    
    solvedBoard = {};

    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            solvedBoard.push_back({i, j});
        }
    }

    cout << "Initial board: " << endl << tiles << endl
         << "Starting search..." << endl << endl;

    node initial{tiles};
    auto start = high_resolution_clock::now();
    node<T> final; 
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

    cout << "Finished board: " << endl << final.tiles
         << "Solution depth: " << final.depth << endl
         << "Time elapsed: " << timeSpent.count() / 1000.0 << " seconds" << endl;
    
    return;
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
        // if (numNodesExpanded % 1000000 == 0) {
        //     cout << "Expanded: " << numNodesExpanded << " Max Queue Size: " << maxQueueSize << " Curr depth: " << currNode.depth << " Time Elapsed:" << duration_cast<milliseconds>(stop - start).count() / 1000.0 << endl << currNode.tiles;
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
            // Calculate manhattan distance by finding difference between actual x and y coordinate and the expected coordinate
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

    // Iterates over the board until it finds the desired number
    for (int i = 0; i < numRows; ++i) {
        numColumns = tiles.at(i).size();
        for (int j = 0; j < numColumns; ++j) {
            if (tiles.at(i).at(j) == num) {
                return {i, j};
            }
        }
    }

    // Not found
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

template <typename T>
pair<int, vector<vector<T> > > parseTestCase(ifstream &fin) {
    if (!fin.is_open()) {
        cerr << "Error: Tried parsing test case without open file" << endl;
        throw std::exception();
    }
    pair<int, vector<vector<T> > > testCase;
    testCase.first = 0;
    // Reach in search number
    fin >> testCase.first;

    string currLine;
    // Remove newline left by >>
    fin.ignore();
    // Read one row of the board
    getline(fin, currLine);

    int currRow = 0;
    T currVal = 0;

    // Iterate until blank line or end of file
    while (!currLine.empty() && fin) {
        stringstream ss(currLine);
        testCase.second.push_back({});

        // Parse individual tiles from row
        ss >> currVal;
        while (ss) {
            testCase.second.at(currRow).push_back(currVal);
            ss >> currVal;
        }

        getline(fin, currLine);
        ++currRow;
    }

    return testCase;
}