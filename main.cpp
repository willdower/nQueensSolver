#include <iostream>
#include <queue>
#include <cmath>

class boardState {
public:
    short numQueens;
    short *board;

    boardState(short n) {
        numQueens = 0;
        board = new short [n]{};
    }

    boardState(boardState *oldBoard, const short & n) {
        numQueens = oldBoard->numQueens;
        board = new short [n]{};
        for (int i=0;i<n;i++) {
            board[i] = oldBoard->board[i];
        }
    }

    ~boardState() {
        delete [] board;
        board = nullptr;
    }

};

bool checkGoalState(boardState *state, const short & n) {

    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i == j) {
                continue;
            }
            if (state->board[i] == state->board[j]) {
                return false;
            }
            //Check diagonal
            if (abs(j-i) == abs(state->board[j]-state->board[i])) {
                return false;
            }
        }
    }

    return true;

}

std::vector<boardState*> unprunedBFSReturnBoards(short n) {
    std::vector<boardState*> goalStates;

    boardState *currentBoard;
    boardState *newBoard;

    std::queue<boardState*> queue;

    queue.push(new boardState(n));

    while (!queue.empty()) {
        currentBoard = queue.front();
        queue.pop();
        if (currentBoard->numQueens == n) {
            if (checkGoalState(currentBoard, n)) {
                goalStates.push_back(currentBoard);
                continue;
            }
            else {
                delete currentBoard;
                currentBoard = nullptr;
                continue;
            }
        }

        for (int i=0;i<n;i++) {
            int validBoard = true;
            for (int j=0;j<currentBoard->numQueens;j++) { //Scanning through current boards queen positions
                if (i == currentBoard->board[j]) {
                    validBoard = false;
                    break;
                }
                if (abs(j-i) == abs(currentBoard->board[j]-currentBoard->board[i])) {
                    validBoard = false;
                    break;
                }
            }

            if (!validBoard) {
                continue;
            }

            newBoard = new boardState(currentBoard, n);
            newBoard->board[newBoard->numQueens] = i;
            newBoard->numQueens++;
            queue.push(newBoard);
        }
        delete currentBoard;
        currentBoard = nullptr;
    }

    return goalStates;

}

int unprunedBFSReturnSolutions(short n, long long & states) {
    int solutions = 0;

    boardState *currentBoard;
    boardState *newBoard;

    std::queue<boardState*> queue;

    queue.push(new boardState(n));

    while (!queue.empty()) {
        currentBoard = queue.front();
        queue.pop();
        states++;
        if (currentBoard->numQueens == n) {
            if (checkGoalState(currentBoard, n)) {
                solutions++;
                continue;
            }
            else {
                delete currentBoard;
                currentBoard = nullptr;
                continue;
            }
        }

        for (int i=0;i<n;i++) {//Potential options of next queen
            int validBoard = true;
            for (int j=0;j<currentBoard->numQueens;j++) { //Scanning through current boards queen positions
                if (i == currentBoard->board[j]) {
                    validBoard = false;
                    break;
                }
                if (currentBoard->board[j] == i) {
                    validBoard = false;
                    break;
                }
            }

            if (!validBoard) {
                continue;
            }

            newBoard = new boardState(currentBoard, n);
            newBoard->board[newBoard->numQueens] = i;
            newBoard->numQueens++;
            queue.push(newBoard);
        }
        delete currentBoard;
        currentBoard = nullptr;
    }

    return solutions;

}

int main() {

    for (int i=4;i<21;i++) {
        if (i < -1) {
            std::vector<boardState*> goalStates = unprunedBFSReturnBoards(i);
            std::cout << i << std::endl;
            for (auto state : goalStates) {
                for (int j=0;j<i;j++) {
                    std::cout << "(" << j << ", " << state->board[j] << ") ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        else {
            long long states = 0;
            std::cout << i << " - " << unprunedBFSReturnSolutions(i, states) << " States: " << states << std::endl;
        }
    }

    /**boardState *test = new boardState(5);
    test->board[0] = 1;
    test->board[1] = 3;
    test->board[2] = 0;
    test->board[3] = 2;
    test->board[4] = 4;
    test->numQueens = 5;

    std::cout << sizeof(test);**/
}