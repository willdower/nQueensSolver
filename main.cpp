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


//PART B

short cost(boardState *state, const short & n) {

    short conflicts = 0;

    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i == j) {
                continue;
            }
            if (state->board[i] == state->board[j]) {
                conflicts++;
            }
            //Check diagonal
            if (abs(j-i) == abs(state->board[j]-state->board[i])) {
                conflicts++;
            }
        }
    }

    return conflicts;

}

short *findBestMove(boardState *state, const short & n, bool canSidewaysMove) {
    short lowestCost = cost(state, n), newCost, lowestRow = -1, lowestCol = -1;
    for (short i=0;i<n;i++) {
        short original = state->board[i];
        for (short j=0;j<n;j++) {
            state->board[i] = j;
            newCost = cost(state, n);
            if (newCost < lowestCost) {
                lowestCost = newCost;
                lowestRow = i;
                lowestCol = j;
            }
        }
        state->board[i] = original;
    }

    if (lowestCol == -1 && canSidewaysMove) {
        while (lowestCol == -1) {
            lowestRow = rand() % n;
            for (int i = 0; i < n; i++) {
                state->board[lowestRow] = i;
                newCost = cost(state, n);
                if (newCost == lowestCost) {
                    lowestCol = i;
                    break;
                }
            }
        }
    }

    auto  *tuple = new short [3];
    tuple[0] = lowestRow;
    tuple[1] = lowestCol;
    tuple[2] = lowestCost;

    return tuple;
}

boardState *generateRandomBoard(const short & n) {
    auto state = new boardState(n);

    for (short i=0;i<n;i++) {
        state->board[i] = rand() % n;
    }

    return state;
}

boardState *hillClimb(const short & n, short allowedSidewaysMoves) {

    boardState *state = generateRandomBoard(n);
    short boardCost = cost(state, n), prevCost;
    short *returnArr;
    short originalAllowedSidewaysMoves = allowedSidewaysMoves;

    if (cost(state, n) == 0) {
        //Random chance success?
        return state;
    }

    while (boardCost != 0) {
        if (allowedSidewaysMoves > 0) {
            returnArr = findBestMove(state, n, true);
        }
        else {
            returnArr = findBestMove(state, n, false);
        }

        if (returnArr[0] == -1 && returnArr[1] == -1) {
            return nullptr;
        }

        prevCost = boardCost;
        state->board[returnArr[0]] = returnArr[1];
        boardCost = cost(state, n);

        if (boardCost == prevCost && allowedSidewaysMoves >= 0) {
            allowedSidewaysMoves--;
        }
        else if (boardCost < prevCost){
            allowedSidewaysMoves = originalAllowedSidewaysMoves;
        }
        else if (boardCost == prevCost && allowedSidewaysMoves < 0) {
            return nullptr;
        }
    }

    return state;
}

void printBoardState(boardState *boardState, const int & n) {
    for (int i=0;i<(n*4)+2;i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
    for (int i=0;i<n;i++) {
        for (int j=-1;j<n;j++) {
            if (j == -1) {
                std::cout << "| ";
                continue;
            }
            if (boardState->board[i] != j) {
                std::cout << "  | ";
            }
            else {
                std::cout << "O | ";
            }
        }
        std::cout << std::endl;
        for (int j=0;j<(n*4)+2;j++) {
            std::cout << "-";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printQueenPositions(boardState *boardState, const int & n) {
    std::cout << "Queen positions at: " << std::endl;

    for (int i=0;i<n;i++) {
        std::cout << "(" << i << ", " << boardState->board[i] << ") ";
    }
    std::cout << std::endl;
}

int main() {
    const short HC_ALLOWED_SIDEWAYS = 0;
    short inputNum;
    std::string input;

    std::cout << "Which algorithm would you like to use? (Type the letters before : exactly)" << std::endl;
    std::cout << "HC: Hill climb" << std::endl;
    std::cin >> input;

    if (input == "HC") {
        std::cout << "Enter n value" << std::endl;
        std::cin >> inputNum;

        auto startTimer = std::chrono::system_clock::now();

        boardState *singleGoalState = hillClimb(inputNum, HC_ALLOWED_SIDEWAYS);

        while (singleGoalState == nullptr) {
            singleGoalState = hillClimb(inputNum, HC_ALLOWED_SIDEWAYS);
        }

        auto endTimer = std::chrono::system_clock::now();

        std::chrono::duration<double> timeTaken = endTimer - startTimer;

        std::cout << "Successful board for N = " << inputNum << " found in " << timeTaken.count() << " seconds." << std::endl;

        if (inputNum > 6) {
            printQueenPositions(singleGoalState, inputNum);
        }
        else {

            printBoardState(singleGoalState, inputNum);

        }


    }

    /**std::cout << "PRUNED BFS to 20" << std::endl << std::endl;

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
    }**/
}