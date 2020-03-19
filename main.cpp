#include <iostream>
#include <queue>
#include <cmath>
#include <chrono>

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

int unprunedBFSReturnSolutions(short n) {
    int solutions = 0;

    boardState *currentBoard;
    boardState *newBoard;

    std::queue<boardState*> queue;

    queue.push(new boardState(n));

    while (!queue.empty()) {
        currentBoard = queue.front();
        queue.pop();
        if (currentBoard->numQueens == n) {
            if (checkGoalState(currentBoard, n)) {
                solutions++;
                delete currentBoard;
                currentBoard = nullptr;
                continue;
            }
            else {
                delete currentBoard;
                currentBoard = nullptr;
                continue;
            }
        }

        for (int i=0;i<n;i++) {//Potential options of next queen
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

std::vector<boardState*> prunedBFSReturnBoards(short n) {

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

    return goalStates;

}

int prunedBFSReturnSolutions(short n) {
    int solutions = 0;

    boardState *currentBoard;
    boardState *newBoard;

    std::queue<boardState*> queue;

    queue.push(new boardState(n));

    while (!queue.empty()) {
        currentBoard = queue.front();
        queue.pop();
        if (currentBoard->numQueens == n) {
            if (checkGoalState(currentBoard, n)) {
                solutions++;
                delete currentBoard;
                currentBoard = nullptr;
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

    std::cout << "PRUNED BFS to 20" << std::endl << std::endl;

    for (int i=4;i<21;i++) {
        if (i < 7) {
            auto startBFS = std::chrono::system_clock::now();
            std::vector<boardState*> goalStates = prunedBFSReturnBoards(i);
            auto endBFS = std::chrono::system_clock::now();
            std::chrono::duration<double> timeTaken = endBFS - startBFS;
            std::cout << i << " - Time Taken: " << timeTaken.count() << std::endl;
            for (auto state : goalStates) {
                for (int j=0;j<i;j++) {
                    std::cout << static_cast<char>(j+97) << state->board[j] << " ";
                }
                std::cout << std::endl;
            }
        }
        else {
            auto startBFS = std::chrono::system_clock::now();
            int solutions = prunedBFSReturnSolutions(i);
            auto endBFS = std::chrono::system_clock::now();
            std::chrono::duration<double> timeTaken = endBFS - startBFS;
            std::cout << i << " - " << solutions << " - Time Taken: " << timeTaken.count();
        }
        std::cout << std::endl;
    }

    std::cout << "UNPRUNED BFS to 20" << std::endl << std::endl;

    for (int i=4;i<21;i++) {
        if (i < 7) {
            auto startBFS = std::chrono::system_clock::now();
            std::vector<boardState*> goalStates = unprunedBFSReturnBoards(i);
            auto endBFS = std::chrono::system_clock::now();
            std::chrono::duration<double> timeTaken = endBFS - startBFS;
            std::cout << i << " - Time Taken: " << timeTaken.count() << std::endl;
            for (auto state : goalStates) {
                for (int j=0;j<i;j++) {
                    std::cout << static_cast<char>(j+97) << state->board[j] << " ";
                }
                std::cout << std::endl;
            }
        }
        else {
            auto startBFS = std::chrono::system_clock::now();
            int solutions = unprunedBFSReturnSolutions(i);
            auto endBFS = std::chrono::system_clock::now();
            std::chrono::duration<double> timeTaken = endBFS - startBFS;
            std::cout << i << " - " << solutions << " - Time Taken: " << timeTaken.count();
        }
        std::cout << std::endl;
    }
}