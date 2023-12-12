#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Size of the puzzle grid
const int PUZZLE_SIZE = 3;

// Structure to represent a state of the puzzle grid
struct PuzzleState {
    vector<vector<int>> grid;
    int heuristicValue;  // Heuristic value based on Manhattan distance

    // Constructor
    PuzzleState(const vector<vector<int>>& puzzle) : grid(puzzle) {
        heuristicValue = calculateHeuristic();
    }

    // Calculate the Manhattan distance heuristic
    int calculateHeuristic() const {
        int distance = 0;
        for (int i = 0; i < PUZZLE_SIZE; ++i) {
            for (int j = 0; j < PUZZLE_SIZE; ++j) {
                if (grid[i][j] != 0) {
                    int targetRow = (grid[i][j] - 1) / PUZZLE_SIZE;
                    int targetCol = (grid[i][j] - 1) % PUZZLE_SIZE;
                    distance += abs(i - targetRow) + abs(j - targetCol);
                }
            }
        }
        return distance;
    }

    // Check if the current state is the goal state
    bool isGoalState() const {
        int targetValue = 0;
        for (int i = 0; i < PUZZLE_SIZE; ++i) {
            for (int j = 0; j < PUZZLE_SIZE; ++j) {
                if (grid[i][j] != targetValue) {
                    return false;
                }
                ++targetValue;
            }
        }
        return true;
    }

    // Check if two puzzle states are equal
    bool operator==(const PuzzleState& other) const {
        return grid == other.grid;
    }
};

// Hash function for PuzzleState (used for unordered_set)
struct PuzzleStateHash {
    size_t operator()(const PuzzleState& state) const {
        size_t hashValue = 0;
        for (const auto& row : state.grid) {
            for (int val : row) {
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
        }
        return hashValue;
    }
};

// Comparison function for priority_queue
struct ComparePuzzleState {
    bool operator()(const PuzzleState& lhs, const PuzzleState& rhs) const {
        return lhs.heuristicValue > rhs.heuristicValue;
    }
};

// Function to display the puzzle state
void displayPuzzleState(const PuzzleState& state) {
    for (const auto& row : state.grid) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "Heuristic Value: " << state.heuristicValue << endl;
    cout << "-----------------" << endl;
}

// Function to perform the greedy search
void performGreedySearch(const PuzzleState& initial) {
    priority_queue<PuzzleState, vector<PuzzleState>, ComparePuzzleState> priorityQueue;
    unordered_set<PuzzleState, PuzzleStateHash> visited;

    priorityQueue.push(initial);

    while (!priorityQueue.empty()) {
        PuzzleState current = priorityQueue.top();
        priorityQueue.pop();

        if (current.isGoalState()) {
            cout << "Goal state reached!" << endl;
            displayPuzzleState(current);
            return;
        }

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            displayPuzzleState(current);

            // Generate possible next states (left, right, up, down moves)
            vector<int> moves = {-1, 0, 1};

            for (int dx : moves) {
                for (int dy : moves) {
                    if (abs(dx) + abs(dy) == 1) {
                        int newX = 0, newY = 0;

                        // Find the position of the empty space (0 value)
                        for (int i = 0; i < PUZZLE_SIZE; ++i) {
                            for (int j = 0; j < PUZZLE_SIZE; ++j) {
                                if (current.grid[i][j] == 0) {
                                    newX = i + dx;
                                    newY = j + dy;
                                    break;
                                }
                            }
                        }

                        // Check if the new position is within bounds
                        if (newX >= 0 && newX < PUZZLE_SIZE && newY >= 0 && newY < PUZZLE_SIZE) {
                            // Create a new state by swapping the empty space and the adjacent tile
                            vector<vector<int>> newGrid = current.grid;
                            swap(newGrid[newX][newY], newGrid[newX - dx][newY - dy]);

                            PuzzleState nextState(newGrid);

                            // Add the new state to the priority queue
                            priorityQueue.push(nextState);
                        }
                    }
                }
            }
        }
    }

    cout << "Goal state not reachable!" << endl;
}

int main() {
    // Initial puzzle state
    vector<vector<int>> initialPuzzle = {
        {8, 0, 6},
        {5, 4, 7},
        {2, 3, 1}
    };

    PuzzleState initialPuzzleState(initialPuzzle);

    cout << "Initial state:" << endl;
    displayPuzzleState(initialPuzzleState);

    cout << "Starting greedy search with Manhattan distance heuristic..." << endl;
    performGreedySearch(initialPuzzleState);

    return 0;
}

