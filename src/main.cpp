#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../include/search.hpp"

using std::cout;
using std::getline;
using std::cin;
using std::endl;
using std::vector;
using std::pair;
using std::ifstream;
using std::stringstream;
using std::string;

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
            cout << endl;
            pair<int, vector<vector<int16_t> > > test = parseTestCase<int16_t>(fin);
            solveBoard(test.second, test.first);
        }
        fin.close();
    }

    return 0;
}

/* Parses a single test case from a file. 
Returns a pair<int, vector<..> > where the int represents the search to use and the vector is the initial board. */
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