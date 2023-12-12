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

struct PuzzleNode {
    PuzzleNode *parent;
    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int positionRow, positionCol;
    int cost;
    int level;
    int heuristic;
};

int displayMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    return 0;
}

PuzzleNode *generateNode(int matrix[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int newX, int newY, int level, PuzzleNode *parent) {
    PuzzleNode *node = new PuzzleNode;
    node->parent = parent;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            node->matrix[i][j] = matrix[i][j];
        }
    }

    int temp = node->matrix[x][y];
    node->matrix[x][y] = node->matrix[newX][newY];
    node->matrix[newX][newY] = temp;
    node->cost = INT_MAX;
    node->level = level;
    node->positionRow = newX;
    node->positionCol = newY;
    return node;
}

int calculateCost(int matrix[MATRIX_SIZE][MATRIX_SIZE], int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    int count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            if (matrix[i][j] != goal[i][j])
                count++;
    return count;
}

int calculateHeuristic(int matrix[MATRIX_SIZE][MATRIX_SIZE], int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    int heuristic = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            if (matrix[i][j] != goal[i][j])
                heuristic++;
    return heuristic;
}

int isWithinBounds(int x, int y) {
    return (x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE);
}

void showPuzzle(PuzzleNode *root) {
    if (root == NULL)
        return;
    showPuzzle(root->parent);
    displayMatrix(root->matrix);
    printf("\n");
}

struct PuzzleNodeComparator {
    bool operator()(const PuzzleNode *lhs, const PuzzleNode *rhs) const {
        return lhs->heuristic > rhs->heuristic;
    }
};

std::string matrixToString(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    std::ostringstream oss;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            oss << matrix[i][j] << " ";
        }
    }
    return oss.str();
}

void solvePuzzle(int initial[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    auto startTime = high_resolution_clock::now();

    priority_queue<PuzzleNode *, vector<PuzzleNode *>, PuzzleNodeComparator> pq;

    unordered_set<string> visited;

    PuzzleNode *root = generateNode(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, goal);
    root->heuristic = calculateHeuristic(initial, goal);
    pq.push(root);

    while (!pq.empty()) {
        PuzzleNode *min = pq.top();
        pq.pop();

        if (min->cost == 0) {
            auto endTime = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(endTime - startTime);

            cout << "Goal state reached in " << totalMoves << " moves.\n";
            cout << "Time taken: " << duration.count() << " milliseconds\n";
            showPuzzle(min);
            return;
        }

        for (int i = 0; i < 4; i++) {
            if (isWithinBounds(min->positionRow + rowOffsets[i], min->positionCol + colOffsets[i])) {
                PuzzleNode *child = generateNode(min->matrix, min->positionRow, min->positionCol, min->positionRow + rowOffsets[i], min->positionCol + colOffsets[i], min->level + 1, min);
                child->cost = calculateCost(child->matrix, goal);
                child->heuristic = calculateHeuristic(child->matrix, goal);

                if (visited.find(matrixToString(child->matrix)) == visited.end()) {
                    pq.push(child);
                    visited.insert(matrixToString(child->matrix));
                    totalMoves++;
                }
            }
        }
    }
}

int main() {
    int initial[MATRIX_SIZE][MATRIX_SIZE];

    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << " Enter the initial state of the puzzle in this format \n";
    cout << "*** 2 3 1 5 6 0 8 4 7 ***\n>> ";

    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            cin >> initial[i][j];

    cout << "The entered initial puzzle is:  \n>> ";
    displayMatrix(initial);
    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << "Solving the Puzzle  \n>> ";

    int startRow = 1, startCol = 2;

    solvePuzzle(initial, startRow, startCol, goalMatrix);

    return 0;
}

