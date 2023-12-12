#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

struct PuzzlePiece {
    vector<vector<int>> configuration;
    int totalCost;
    int heuristicValue;
    int movesTaken;
    const PuzzlePiece* previousPiece;

    PuzzlePiece(const vector<vector<int>>& arrangement, int cost, int heuristic, int moves, const PuzzlePiece* previous)
        : configuration(arrangement), totalCost(cost), heuristicValue(heuristic), movesTaken(moves), previousPiece(previous) {}

    bool operator==(const PuzzlePiece& other) const {
        return configuration == other.configuration;
    }
};

struct PuzzlePieceHash {
    size_t operator()(const PuzzlePiece& piece) const {
        size_t hash = 0;
        for (const auto& row : piece.configuration) {
            for (int num : row) {
                hash ^= hash << 6 ^ hash >> 2 ^ size_t(num) + 0x9e3779b9 + (hash << 14) + (hash >> 7);
            }
        }
        return hash;
    }
};

struct PuzzlePieceComparator {
    bool operator()(const PuzzlePiece& a, const PuzzlePiece& b) const {
        return a.totalCost + a.heuristicValue > b.totalCost + b.heuristicValue;
    }
};

void displayPuzzle(const vector<vector<int>>& arrangement) {
    for (const auto& row : arrangement) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
    cout << endl;
}

pair<int, int> findNumberPosition(const vector<vector<int>>& arrangement, int number) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (arrangement[i][j] == number) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

int calculateManhattan(const PuzzlePiece& piece) {
    int distance = 0;
    for (int num = 1; num <= 8; ++num) {
        pair<int, int> currentPos = findNumberPosition(piece.configuration, num);
        pair<int, int> goalPos = {(num - 1) / 3, (num - 1) % 3};
        distance += abs(currentPos.first - goalPos.first) + abs(currentPos.second - goalPos.second);
    }
    return distance;
}

bool isPositionValid(int i, int j) {
    return i >= 0 && i < 3 && j >= 0 && j < 3;
}

vector<PuzzlePiece> generatePuzzlePieces(const PuzzlePiece& piece);

void runAStar(const vector<vector<int>>& initialConfiguration);

int main() {
    vector<vector<int>> startConfiguration = {{1, 2, 3}, {0, 4, 6}, {7, 5, 8}};

    cout << "Initial state:" << endl;
    displayPuzzle(startConfiguration);

    runAStar(startConfiguration);

    return 0;
}

vector<PuzzlePiece> generatePuzzlePieces(const PuzzlePiece& piece) {
    vector<PuzzlePiece> pieces;
    pair<int, int> emptyPosition = findNumberPosition(piece.configuration, 0);

    const int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (const auto& move : moves) {
        int newI = emptyPosition.first + move[0];
        int newJ = emptyPosition.second + move[1];

        if (isPositionValid(newI, newJ)) {
            vector<vector<int>> newConfiguration = piece.configuration;
            swap(newConfiguration[emptyPosition.first][emptyPosition.second], newConfiguration[newI][newJ]);

            int newCost = piece.totalCost + 1;
            int newHeuristic = calculateManhattan({newConfiguration, 0, 0, 0, nullptr});
            int newMoves = piece.movesTaken + 1;

            pieces.emplace_back(newConfiguration, newCost, newHeuristic, newMoves, &piece);
        }
    }

    return pieces;
}

void runAStar(const vector<vector<int>>& initialConfiguration) {
    PuzzlePiece initialState{initialConfiguration, 0, calculateManhattan({initialConfiguration, 0, 0, 0, nullptr}), 0, nullptr};

    priority_queue<PuzzlePiece, vector<PuzzlePiece>, PuzzlePieceComparator> openPieces;

    unordered_set<PuzzlePiece, PuzzlePieceHash> closedPieces;

    openPieces.push(initialState);

    while (!openPieces.empty()) {
        PuzzlePiece currentPiece = openPieces.top();
        openPieces.pop();

        if (currentPiece.heuristicValue == 0) {
            cout << "Goal state reached in " << currentPiece.movesTaken << " moves." << endl;
            cout << "Solution:" << endl;

            while (currentPiece.previousPiece != nullptr) {
                displayPuzzle(currentPiece.configuration);
                currentPiece = *currentPiece.previousPiece;
            }
            displayPuzzle(initialState.configuration);
            return;
        }

        closedPieces.insert(currentPiece);

        vector<PuzzlePiece> successors = generatePuzzlePieces(currentPiece);

        for (const PuzzlePiece& successor : successors) {
            if (closedPieces.find(successor) == closedPieces.end()) {
                openPieces.push(successor);
            }
        }
    }

    cout << "Goal state not reachable." << endl;
}

