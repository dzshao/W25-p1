#ifndef BOARD_UTIL_HPP
#define BOARD_UTIL_HPP

#include <vector>
#include <iostream>

using std::vector;
using std::ostream;
using std::endl;
using std::pair;

/* Returns the coordinates of the '0' space on a board. */
template <typename T>
pair<int, int> findBlank(const vector<vector<T> > &);

/* Returns the coordinates of any given tile on a board*/
template <typename T>
pair<int, int> findTile(const vector<vector<T> > &, int);

/* Print out board. */
template <typename T>
ostream& operator<<(ostream& out, const vector<vector<T> > &rhs);

/* Returns the coordinates of the '0' space on a board. */
template <typename T>
pair<int, int> findBlank(const vector<vector<T> > &tiles) {
    return findTile(tiles, 0);
}

/* Returns the coordinates of any given tile on a board*/
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

/* Print out board. */
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

#endif
