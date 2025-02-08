# W25 Project 1

# Usage

# Running Custom Test Cases
Test cases are stored in .txt files. The first line in each test case is a single integer that indicates the search/heursitic function to use for that case.
```
1 = Uniform Cost Search
2 = Misplaced Tile Heuristic
3 = Manhattan Distance 
```

The next `n` lines represent the board, with each tile separated by a space, and each row separated by a newline. Rows can be of different lengths, and any number of rows can be entered (as long as the total number of tiles is less than 32768).
```
4 5 6
7 8 0
1 2 3
```

Multiple test cases can be included in the same file, separated by a blank line.

```
1
0 6 7
2 5 4
3 8 1

3
13 10 9 7
2 1 0 12
15 11 14 3
4 8 6 5
```

The above example uses the Uniform Cost Search to solve the 3x3 puzzle, then uses Manhattan Distance for solving the following 4x4 puzzle.