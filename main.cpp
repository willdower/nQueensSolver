#include <iostream>
#include <queue>
#include <cmath>
#include <chrono>
#include <limits>

//Shorts used in many places instead of ints to conserve space

class boardState {
public:
    short numQueens;
    short *board; //Array index is row, value at index is column

    explicit boardState(short n) {
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
            if (abs(j-i) == abs(state->board[j]-state->board[i])) { //If absolute distance between rows and cols is
                return false;                                       //equal, two queens are in same diagonal
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
        currentBoard = queue.front(); //Pop state
        queue.pop();
        if (currentBoard->numQueens == n) {
            if (checkGoalState(currentBoard, n)) { //Check if goal
                goalStates.push_back(currentBoard);
                continue;
            }
            else {
                delete currentBoard;
                currentBoard = nullptr; //Set to nullptr to avoid double deletion or incorrect access
                continue;
            }
        }

        for (int i=0;i<n;i++) {
            newBoard = new boardState(currentBoard, n); //Push newly explored nodes to queue
            newBoard->board[newBoard->numQueens] = i;
            newBoard->numQueens++;
            queue.push(newBoard);
        }
        delete currentBoard; //Delete old board to save memory
        currentBoard = nullptr;
    }

    return goalStates;

}

int unprunedBFSReturnSolutions(short n) { //Same as above, see comments on function above
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
                if (i == currentBoard->board[j]) { //Check same column
                    validBoard = false;
                    break;
                }
                if (abs(currentBoard->board[j]-i) == abs((currentBoard->numQueens)-j)) { //Check same diagonal
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
                if (i == currentBoard->board[j]) { //Check same column
                    validBoard = false;
                    break;
                }
                if (abs(currentBoard->board[j]-i) == abs((currentBoard->numQueens)-j)) { //Check same diagonal
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
            if (state->board[i] == state->board[j]) { //Check column conflict
                conflicts++;
            }
            if (abs(j-i) == abs(state->board[j]-state->board[i])) { //Check diagonal conflict
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
            state->board[i] = j; //Set column to new value
            newCost = cost(state, n);
            if (newCost < lowestCost) { //Check if cost of new value better
                lowestCost = newCost; //If yes, set as best neighbour
                lowestRow = i;
                lowestCol = j;
            }
        }
        state->board[i] = original; //Return to original for further checks
    }

    if (lowestCol == -1 && canSidewaysMove) { //If none better and can sidewaysmove
        while (lowestCol == -1) { //Pick a random row and check it for sideways moves, repeat if not found
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
        //Small chance of successfully randomly generating solution?
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
            return nullptr; //No solution found
        }

        prevCost = boardCost;
        state->board[returnArr[0]] = returnArr[1];
        boardCost = cost(state, n);

        if (boardCost == prevCost && allowedSidewaysMoves >= 0) {
            allowedSidewaysMoves--; //Sideways move no better, decrease allowed moves
        }
        else if (boardCost < prevCost){
            allowedSidewaysMoves = originalAllowedSidewaysMoves; //Reset sideways moves
        }
        else if (boardCost == prevCost && allowedSidewaysMoves < 0) {
            return nullptr; //No solution found
        }
    }

    return state;
}

boardState *stochasticHillClimb(const short & n, const long & checkOnFails) {

    boardState *state = generateRandomBoard(n);
    short boardCost = cost(state, n);
    short *returnArr;


    if (cost(state, n) == 0) {
        //Small chance of successfully randomly generating solution?
        return state;
    }

    int failCount = 0;

    while (boardCost != 0) {
        int randomRow, randomCol;

        randomRow = rand() % n;
        randomCol = rand() % n;
        int original = state->board[randomRow];
        state->board[randomRow] = randomCol;
        int newCost = cost(state, n);
        if (newCost < boardCost) {
            boardCost = newCost;
        }
        else {
            state->board[randomRow] = original;
            failCount++;
        }

        if (failCount > checkOnFails) { //After certain number of fails, check if stuck in unrecoverable state
            returnArr = findBestMove(state, n, false);
            if (returnArr[0] == -1) {
                return nullptr; //No solution found
            }
            else {
                failCount = 0; //Not unrecoverable, just unlucky
            }
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

bool acceptance(const int & newCost, const int & currentCost, const double & temperature) {
    if (newCost < currentCost) {
        return true;
    }

    double chance = exp(((currentCost - newCost)*20)/temperature); //*20 increases weight of cost difference

    auto random = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

    if (chance > random) {
        return true;
    }
    else {
        return false;
    }
}

boardState *simulatedAnnealing(short n, double temperature, const double & coolingRate, const int & iterationThreshold) {
    boardState *state = generateRandomBoard(n);
    int currentCost = n*n; //Max possible cost
    int newCost;
    int iterations = 0;

    const double coolingMult = 1 - coolingRate;

    while (currentCost != 0) {
        srand(time(nullptr)); //Update random seed every loop for better randomness
        while (temperature > 1) {
            iterations++;
            int randomRow = rand() % n;
            int randomCol = rand() % n;

            int original = state->board[randomRow];
            state->board[randomRow] = randomCol; //Change random queen

            newCost = cost(state, n);
            if (acceptance(newCost, currentCost, temperature)) { //If better, new cost updated
                currentCost = newCost;
            }
            else {
                state->board[randomRow] = original; //If worse, revert change
            }
            if (iterations > iterationThreshold) { //Change temp if static iterations has expired
                temperature = temperature*coolingMult;
            }
        }
        temperature = 10;
        //If temperature runs down but doesn't find solution, bump temp back up to beat small bumps on the way to
        //global optimum
    }
    return state;
}

int main() {
    srand(time(nullptr));
    const short HC_ALLOWED_SIDEWAYS = 0; //Allowed sideways hillclimb moves
    const long SHC_FAIL_CHECK = 1000; //Stochastic hillclimb fail check after this many failures
    const int SA_TEMPERATURE = 100000; //Starting temp for simulated annealing
    const double SA_COOLING_RATE = 0.0001; //Cooling rate for simulated annealing
    const int SA_STATIC_ITERS = 10000; //Number of static iterations for simulated annealing
    short inputNum;
    int inputNum2;
    std::string input;
    bool successfulInput = false;

    std::cout << std::endl;
    std::cout << "Which algorithm would you like to use? (Type the letters before : exactly)" << std::endl;
    std::cout << std::endl;
    std::cout << "BFS: Breadth-first search" << std::endl;
    std::cout << "PBFS: Pruned breadth-first search" << std::endl;
    std::cout << std::endl;
    std::cout << "RBFS: Range breadth-first search (do BFS for all numbers between two values)" << std::endl;
    std::cout << "RPBFS: Range pruned breadth-first search (do PBFS for all numbers between two values)" << std::endl;
    std::cout << std::endl;
    std::cout << "HC: Hill climb" << std::endl;
    std::cout << "RHC: Range hill climb (do HC for all numbers between two values)" << std::endl;
    std::cout << "SHC: Stochastic hill climb" << std::endl;
    std::cout << "SA: Simulated Annealing" << std::endl;
    std::cout << "RSA: Range simulated annealing (do SA for all numbers between two values)" << std::endl;
    std::cout << std::endl << "EX: Exit program." << std::endl;

    while (!successfulInput) {
        std::cin >> input;
        if (input == "HC") {
            std::cout << "Enter n value" << std::endl;
            while (!successfulInput) {
                std::cin >> inputNum;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    successfulInput = true;
                }
            }
            std::cout << "Working..." << std::endl;

            if (inputNum == 2 || inputNum == 3) { //2 and 3 are only N values to have 0 solutions
                std::cout << inputNum << " has 0 solutions." << std::endl;
                break;
            }

            auto startTimer = std::chrono::system_clock::now();

            boardState *singleGoalState = hillClimb(inputNum, HC_ALLOWED_SIDEWAYS);

            while (singleGoalState == nullptr) {
                singleGoalState = hillClimb(inputNum, HC_ALLOWED_SIDEWAYS);
            }

            auto endTimer = std::chrono::system_clock::now();

            std::chrono::duration<double> timeTaken = endTimer - startTimer;

            std::cout << "Successful board for N = " << inputNum << " found in " << timeTaken.count() << " seconds."
                      << std::endl;

            if (inputNum > 6) {
                printQueenPositions(singleGoalState, inputNum);
            } else {

                printBoardState(singleGoalState, inputNum);

            }


        }
        else if (input == "RHC") {
            std::cout << "Enter lowest n value" << std::endl;
            while (1) {
                std::cin >> inputNum;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    break;
                }
            }

            while (1) {
                std::cout << "Enter highest n value" << std::endl;

                std::cin >> inputNum2;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    successfulInput = true;
                    break;
                }
            }

            std::cout << "Working..." << std::endl;
            for (int i=inputNum;i<=inputNum2;i++) {

                if (i == 2 || i == 3) { //2 and 3 are only N values to have 0 solutions
                    std::cout << i << " has 0 solutions." << std::endl;
                    continue;
                }

                auto startTimer = std::chrono::system_clock::now();

                boardState *singleGoalState = hillClimb(i, HC_ALLOWED_SIDEWAYS);

                while (singleGoalState == nullptr) {
                    singleGoalState = hillClimb(i, HC_ALLOWED_SIDEWAYS);
                }

                auto endTimer = std::chrono::system_clock::now();

                std::chrono::duration<double> timeTaken = endTimer - startTimer;

                std::cout << "Successful board for N = " << i << " found in " << timeTaken.count() << " seconds."
                          << std::endl;

                if (i > 6) {
                    printQueenPositions(singleGoalState, i);
                } else {

                    printBoardState(singleGoalState, i);

                }
            }
        }
        else if (input == "SHC") {
            std::cout << "Enter n value" << std::endl;
            while (!successfulInput) {
                std::cin >> inputNum;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    successfulInput = true;
                }
            }
            std::cout << "Working..." << std::endl;

            if (inputNum == 2 || inputNum == 3) { //2 and 3 are only N values to have 0 solutions
                std::cout << inputNum << " has 0 solutions." << std::endl;
                break;
            }

            auto startTimer = std::chrono::system_clock::now();

            boardState *singleGoalState = stochasticHillClimb(inputNum, SHC_FAIL_CHECK);

            while (singleGoalState == nullptr) {
                singleGoalState = stochasticHillClimb(inputNum, SHC_FAIL_CHECK);
            }

            auto endTimer = std::chrono::system_clock::now();

            std::chrono::duration<double> timeTaken = endTimer - startTimer;

            std::cout << "Successful board for N = " << inputNum << " found in " << timeTaken.count() << " seconds."
                      << std::endl;

            if (inputNum > 6) {
                printQueenPositions(singleGoalState, inputNum);
            } else {

                printBoardState(singleGoalState, inputNum);

            }


        }
        else if (input == "SA") {
            std::cout << "Enter n value" << std::endl;
            while (!successfulInput) {
                std::cin >> inputNum;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    successfulInput = true;
                }
            }
            std::cout << "Working..." << std::endl;

            if (inputNum == 2 || inputNum == 3) { //2 and 3 are only N values to have 0 solutions
                std::cout << inputNum << " has 0 solutions." << std::endl;
                break;
            }

            auto startTimer = std::chrono::system_clock::now();

            boardState *singleGoalState = simulatedAnnealing(inputNum, SA_TEMPERATURE, SA_COOLING_RATE, SA_STATIC_ITERS);

            while (singleGoalState == nullptr) {
                singleGoalState = simulatedAnnealing(inputNum, SA_TEMPERATURE, SA_COOLING_RATE, SA_STATIC_ITERS);
            }

            auto endTimer = std::chrono::system_clock::now();

            std::chrono::duration<double> timeTaken = endTimer - startTimer;

            std::cout << "Successful board for N = " << inputNum << " found in " << timeTaken.count() << " seconds."
                      << std::endl;

            if (inputNum > 6) {
                printQueenPositions(singleGoalState, inputNum);
            } else {

                printBoardState(singleGoalState, inputNum);

            }


        }
        else if (input == "RSA") {
            std::cout << "Enter lowest n value" << std::endl;
            while (1) {
                std::cin >> inputNum;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    break;
                }
            }

            while (1) {
                std::cout << "Enter highest n value" << std::endl;

                std::cin >> inputNum2;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    successfulInput = true;
                    break;
                }
            }
            std::cout << "Working..." << std::endl;

            for (int i=inputNum;i<=inputNum2;i++) {

                if (i == 2 || i == 3) { //2 and 3 are only N values to have 0 solutions
                    std::cout << i << " has 0 solutions." << std::endl;
                    continue;
                }

                auto startTimer = std::chrono::system_clock::now();

                boardState *singleGoalState = simulatedAnnealing(i, SA_TEMPERATURE, SA_COOLING_RATE, SA_STATIC_ITERS);

                while (singleGoalState == nullptr) {
                    singleGoalState = simulatedAnnealing(i, SA_TEMPERATURE, SA_COOLING_RATE, SA_STATIC_ITERS);
                }

                auto endTimer = std::chrono::system_clock::now();

                std::chrono::duration<double> timeTaken = endTimer - startTimer;

                std::cout << "Successful board for N = " << i << " found in " << timeTaken.count() << " seconds." << std::endl;

                if (i > 6) {
                    printQueenPositions(singleGoalState, i);
                } else {
                    printBoardState(singleGoalState, i);

                }
            }
        }
        else if (input == "BFS") {
            std::cout << "Enter n value" << std::endl;
            std::cin >> inputNum;
            if (std::cin.fail() || inputNum == 0) {
                std::cout << "Invalid input. Please try again." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                successfulInput = true;
            }
            std::cout << "Working..." << std::endl;

            if (inputNum < 7) {
                auto startBFS = std::chrono::system_clock::now();
                std::vector<boardState*> goalStates = unprunedBFSReturnBoards(inputNum);
                auto endBFS = std::chrono::system_clock::now();
                std::chrono::duration<double> timeTaken = endBFS - startBFS;
                std::cout << inputNum << " - Solutions: " << goalStates.size() << " - Time Taken: " << timeTaken.count() << std::endl;
                for (auto state : goalStates) {
                    printBoardState(state, inputNum);
                    std::cout << std::endl;
                }
            }
            else {
                auto startBFS = std::chrono::system_clock::now();
                int solutions = unprunedBFSReturnSolutions(inputNum);
                auto endBFS = std::chrono::system_clock::now();
                std::chrono::duration<double> timeTaken = endBFS - startBFS;
                std::cout << inputNum << " - " << solutions << " - Time Taken: " << timeTaken.count();
            }
            std::cout << std::endl;
        }
        else if (input == "PBFS") {
            std::cout << "Enter n value" << std::endl;
            std::cin >> inputNum;
            if (std::cin.fail() || inputNum == 0) {
                std::cout << "Invalid input. Please try again." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                successfulInput = true;
            }
            std::cout << "Working..." << std::endl;

            if (inputNum < 7) {
                auto startBFS = std::chrono::system_clock::now();
                std::vector<boardState*> goalStates = prunedBFSReturnBoards(inputNum);
                auto endBFS = std::chrono::system_clock::now();
                std::chrono::duration<double> timeTaken = endBFS - startBFS;
                std::cout << inputNum << " - Solutions: " << goalStates.size() << " - Time Taken: " << timeTaken.count() << std::endl;
                for (auto state : goalStates) {
                    printBoardState(state, inputNum);
                    std::cout << std::endl;
                }
            }
            else {
                auto startBFS = std::chrono::system_clock::now();
                int solutions = prunedBFSReturnSolutions(inputNum);
                auto endBFS = std::chrono::system_clock::now();
                std::chrono::duration<double> timeTaken = endBFS - startBFS;
                std::cout << inputNum << " - " << solutions << " - Time Taken: " << timeTaken.count();
            }
            std::cout << std::endl;
        }
        else if (input == "RBFS") {
            std::cout << "Enter lowest n value" << std::endl;
            while (1) {
                std::cin >> inputNum;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    break;
                }
            }

            while (1) {
                std::cout << "Enter highest n value" << std::endl;

                std::cin >> inputNum2;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    successfulInput = true;
                    break;
                }
            }
            std::cout << "Working..." << std::endl;

            for (int i=inputNum;i<=inputNum2;i++) {
                if (i < 7) {
                    auto startBFS = std::chrono::system_clock::now();
                    std::vector<boardState*> goalStates = unprunedBFSReturnBoards(i);
                    auto endBFS = std::chrono::system_clock::now();
                    std::chrono::duration<double> timeTaken = endBFS - startBFS;
                    std::cout << i << " - Solutions: " << goalStates.size() << " - Time Taken: " << timeTaken.count() << std::endl;
                    for (auto state : goalStates) {
                        printBoardState(state, i);
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
        else if (input == "RPBFS") {
            std::cout << "Enter lowest n value" << std::endl;
            while (1) {
                std::cin >> inputNum;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    break;
                }
            }

            while (1) {
                std::cout << "Enter highest n value" << std::endl;

                std::cin >> inputNum2;
                if (std::cin.fail() || inputNum == 0) {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    successfulInput = true;
                    break;
                }
            }
            std::cout << "Working..." << std::endl;

            for (int i=inputNum;i<=inputNum2;i++) {
                if (i < 7) {
                    auto startBFS = std::chrono::system_clock::now();
                    std::vector<boardState*> goalStates = prunedBFSReturnBoards(i);
                    auto endBFS = std::chrono::system_clock::now();
                    std::chrono::duration<double> timeTaken = endBFS - startBFS;
                    std::cout << i << " - Solutions: " << goalStates.size() << " - Time Taken: " << timeTaken.count() << std::endl;
                    for (auto state : goalStates) {
                        printBoardState(state, i);
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
        }
        else if (input == "EX") {
            return 0;
        }
        else {
            std::cout << "Invalid entry." << std::endl;
            std::cout << input << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}