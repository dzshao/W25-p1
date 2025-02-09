# W25 Project 1
Sliding tile puzzle solver. Created using C++17.

# Usage
Compiled using g++ running the command:

```
g++ ./src/main.cpp -std=c++17 -o tile_solver
```

Usage of the program:

```
./tile_solver [test_case.txt]
```

If no test case is provided, a default case is automatically used:
```
0 1 6
5 3 2
4 7 8
```

You are then prompted to select which search algorithm to use, enter 1 for Uniform Distance, 2 for Misplaced Tiles, and 3 for Manhattan Distance. 

After either completing the puzzle or failing by visiting all possible states/timing out, the program prints the total number of nodes expanded, maximum queue size, and time elapsed.

# Running Custom Test Cases
Test cases are stored in .txt files. The first line in each test case is a single integer that indicates the search/heursitic function to use for that case.
```
1 = Uniform Cost Search
2 = Misplaced Tiles Heuristic
3 = Manhattan Distance 
```

The next `n` lines represent the board, with each tile delimited by a space, and each row delimited by a newline. Rows can be of variable, and any number of rows can be entered (as long as the total number of tiles is less than 32768).
```
4 5 6
7 8 0
1 2 3
```
> Boards greater than 3x3 also work
```
1 9 7 4 5
6 3 12 0 2
11 8 24 13 15
16 18 22 19 10
21 17 23 14 20
```

Multiple test cases are delimited by a blank line.

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

The above example uses the Uniform Cost Search to solve the 3x3 puzzle, then uses A* with the Manhattan Distance heuristic for solving the following 4x4 puzzle.