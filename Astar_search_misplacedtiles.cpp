 #include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace chrono;

#define MATRIX_SIZE 3
int totalMoves = 0;
int rowOffsets[] = {1, 0, -1, 0};
int colOffsets[] = {0, -1, 0, 1};

int goalMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8}
};

struct PuzzleTile {
    PuzzleTile *ancestor;
    int layout[MATRIX_SIZE][MATRIX_SIZE];
    int xPos, yPos;
    int cost;
    int level;
};

int displayPuzzleMatrix(int layout[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++)
            printf("%d ", layout[i][j]);
        printf("\n");
    }
    return 0;
}

PuzzleTile *createPuzzleTile(int layout[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int newX, int newY, int level, PuzzleTile *ancestor) {
    PuzzleTile *node = new PuzzleTile;
    node->ancestor = ancestor;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            node->layout[i][j] = layout[i][j];
        }
    }

    int temp = node->layout[x][y];
    node->layout[x][y] = node->layout[newX][newY];
    node->layout[newX][newY] = temp;
    node->cost = INT_MAX;
    node->level = level;
    node->xPos = newX;
    node->yPos = newY;
    return node;
}

int calculateCost(int layout[MATRIX_SIZE][MATRIX_SIZE], int target[MATRIX_SIZE][MATRIX_SIZE]) {
    int count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            if (layout[i][j] != target[i][j]) {
                count++;
            }
    return count;
}

int isWithinBounds(int x, int y) {
    return (x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE);
}

void displayPuzzleState(PuzzleTile *root) {
    if (root == NULL)
        return;
    displayPuzzleState(root->ancestor);
    displayPuzzleMatrix(root->layout);
    printf("\n");
}

struct PuzzleComparison {
    bool operator()(const PuzzleTile *lhs, const PuzzleTile *rhs) const {
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
    }
};

std::string stringifyPuzzleMatrix(int layout[MATRIX_SIZE][MATRIX_SIZE]) {
    std::ostringstream oss;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            oss << layout[i][j] << " ";
        }
    }
    return oss.str();
}

void solvePuzzle(int initial[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int target[MATRIX_SIZE][MATRIX_SIZE]) {
    auto startTime = high_resolution_clock::now();

    priority_queue<PuzzleTile *, vector<PuzzleTile *>, PuzzleComparison> puzzleQueue;

    unordered_set<string> visitedStates;

    PuzzleTile *root = createPuzzleTile(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, target);
    puzzleQueue.push(root);

    while (!puzzleQueue.empty()) {
        PuzzleTile *minTile = puzzleQueue.top();
        puzzleQueue.pop();

        if (minTile->cost == 0) {
            auto endTime = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(endTime - startTime);

            cout << "Puzzle solved in " << totalMoves << " moves.\n";
            cout << "Time taken: " << duration.count() << " milliseconds\n";
            displayPuzzleState(minTile);
            return;
        }

        for (int i = 0; i < 4; i++) {
            if (isWithinBounds(minTile->xPos + rowOffsets[i], minTile->yPos + colOffsets[i])) {
                PuzzleTile *childTile = createPuzzleTile(minTile->layout, minTile->xPos, minTile->yPos, minTile->xPos + rowOffsets[i], minTile->yPos + colOffsets[i], minTile->level + 1, minTile);
                childTile->cost = calculateCost(childTile->layout, target);

                if (visitedStates.find(stringifyPuzzleMatrix(childTile->layout)) == visitedStates.end()) {
                    puzzleQueue.push(childTile);
                    visitedStates.insert(stringifyPuzzleMatrix(childTile->layout));
                    totalMoves++;
                }
            }
        }
    }
}

int main() {
    int initialPuzzle[MATRIX_SIZE][MATRIX_SIZE];

    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << " Enter the starting state of the puzzle in the following format: \n";
    cout << "*** 2 3 1 5 6 0 8 4 7 ***\n>> ";

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            cin >> initialPuzzle[i][j];

    cout << "Starting state of the puzzle is:  \n>> ";
    displayPuzzleMatrix(initialPuzzle);
    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << "Solving the puzzle, please wait  \n>> ";

    int initialX = 1, initialY = 2;

    solvePuzzle(initialPuzzle, initialX, initialY, goalMatrix);

    return 0;
}

