//
// Created by William Dower on 2020-03-17.
//

#include "Old.h"

#include <iostream>
#include <queue>

class boardState {
public:
    short n;
    short numQueens;
    std::vector<std::vector<bool>> board;

    boardState(short boardSize) {
        numQueens = 0;
        n = boardSize;
        board = std::vector<std::vector<bool>>(n, std::vector<bool>(n));
        for (int i=0;i<n;i++) {
            for (int j=0;j<n;j++) {
                board[i].push_back(false);
            }
        }
    }

    boardState(boardState *oldBoardState) {
        numQueens = oldBoardState->numQueens;
        n = oldBoardState->n;
        board = std::vector<std::vector<bool>>(n, std::vector<bool>(n));
        for (int i=0;i<n;i++) {
            for (int j=0;j<n;j++) {
                board[i][j] = oldBoardState->board[i][j];
            }
        }
    }

    ~boardState() {
        for (int i=0;i<n;i++) {
            board[i].clear();
        }
        board.clear();
    }
};

bool checkGoalState(boardState *currentBoard) {
    //Check if there is the right number of queens
    if (currentBoard->n != currentBoard->numQueens) {
        return false;
    }
    //Check all cells
    for (int i=0;i<currentBoard->n;i++) {
        for (int j=0;j<currentBoard->n;j++) {
            if (currentBoard->board[i][j]) { //If queen found in cell
                //Don't need to check if any queens in the same row - this will never happen in this BFS search

                //Check if any queens in the same column
                for (int k=i+1;k<currentBoard->n;k++) {
                    if (currentBoard->board[k][j]) {
                        return false;
                    }
                }

                //Check if any queens in same diagonal
                int x = i, y = j;
                while (x != 0 && y != 0) {
                    x--;
                    y--;
                }
                while (x < currentBoard->n && y < currentBoard->n) {
                    if (x == i && y == j) { //Don't include place of original queen
                        x++;
                        y++;
                        continue;
                    }
                    if (currentBoard->board[x][y]) {
                        return false;
                    }
                    x++;
                    y++;
                }

                x = i, y = j;
                while (x < currentBoard->n-1 && y != 0) {
                    x++;
                    y--;
                }
                while (x > 0 && y < currentBoard->n) {
                    if (x == i && y == j) { //Don't include place of original queen
                        x--;
                        y++;
                        continue;
                    }
                    if (currentBoard->board[x][y]) {
                        return false;
                    }
                    x--;
                    y++;
                }

            }
        }
    }

    return true;
}

std::vector<boardState*> unprunedBFS(short n, int & states) {

    std::vector<boardState*> goalStates;

    std::queue<boardState*> queue;
    boardState *current;
    boardState *newBoardState;

    queue.push(new boardState(n)); //Push an empty board

    while (!queue.empty()) {
        current = queue.front();
        queue.pop();
        if (checkGoalState(current)) {
            goalStates.push_back(current);
            continue;
            //If its a goal, end of this branch
        }
        else if (current->numQueens == n) {
            delete current;
            continue;
            //If its got n queens but isn't a solution, end of this branch
        }

        for (int i=0;i<current->n;i++) {
            newBoardState = new boardState(current);
            newBoardState->board[newBoardState->numQueens][i] = true;
            newBoardState->numQueens++;
            queue.push(newBoardState);
            states++;
        }

    }

    return goalStates;

}

int main() {

    /**boardState testBoard = boardState(4);

    testBoard.arr[0][1] = true;
    testBoard.arr[1][3] = true;
    testBoard.arr[2][0] = true;
    testBoard.arr[3][2] = true;
    testBoard.numQueens = 4;**/

    std::vector<boardState*> goalStates;
    int states = 0;

    for (int n=4;n<10;n++) {
        goalStates = unprunedBFS(n, states);

        if (n <= 6) {
            std::cout << n << " " << states << std::endl << std::endl;
            for (auto state : goalStates) {
                for (int i = 0; i < state->n; i++) {
                    for (int j = 0; j < state->n; j++) {
                        if (state->board[i][j]) {
                            std::cout << "(" << i << ", " << j << ") ";
                        }
                    }
                }
                std::cout << std::endl;
            }

        }
        else {
            std::cout << n << " " << goalStates.size() << " " << states << std::endl;
        }

        for (auto state : goalStates) {
            delete state;
        }
        while (!goalStates.empty()) {
            goalStates.pop_back();
        }
    }

    /**int test = 0;

    boardState testBoardState = boardState(9);

    std::cout << sizeof(testBoardState);**/

    return 0;
}

