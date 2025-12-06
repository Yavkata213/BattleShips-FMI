#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace std;

void saveGameStateToFile(char[15][15], char[15][15], int, bool);

void loadGameFromFile(char[15][15], char[15][15], int, bool);

void startNewGame(char[15][15], char[15][15], int, bool, bool);

void printUI(char[15][15], char[15][15], int);

int main() {
    srand(time(0));
    //1. Choose to load/start new game
    //2. Choose difficulty: 
    //      - Calm Waters (дъска 10x10)
    //      - Rough Seas (дъска 12x12)
    //      - Storm of Steel (дъска 15x15)
    //3. Fill computer board
    //4. Fill player board
    //5. Play out game

    bool isPlayerTurn;

    string choice;

    int boardSize = 10;

    char computerBoard[15][15];
    char playerBoard[15][15];

    //TODO: load or start new game depending on choice; automatic player input

    bool manualPlayerInput = false;

    startNewGame(computerBoard, playerBoard, boardSize, manualPlayerInput, isPlayerTurn);

    printUI(computerBoard, playerBoard, boardSize);

    bool isGameOver = false;

    //TODO: game cycle; playTurn(..., isPlayerTurn); at the end of each turn check if the game is ended; when end, restart

    //retry input if invalid
    while (!isGameOver) {
        printUI(computerBoard, playerBoard, boardSize);
    }
}

void isValidPlacement(char board[15][15], int shipLength, bool isHorizontal) {

}

void automaticallyFillBoard(char board[15][15], int boardSize) {
    //TODO: depending on board size place: 4 - 1 tile ships, 3 - 2 tile ships, 2 - 3 tile ships, 1 - 4 tile ship

    //TODO: generate a random tile in the board and check if valid

    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0; col < boardSize; col++)
        {
            board[row][col] = '~';
        }
    }
}

void startNewGame(char computerBoard[15][15], char playerBoard[15][15], int boardSize, bool manualInput, bool isPlayerTurn) {
    automaticallyFillBoard(computerBoard, boardSize);

    if (!manualInput) {
        automaticallyFillBoard(playerBoard, boardSize);
    } else {
        //TODO: manual ship input, example: 0(first coord) 2(second coord) 0/1/true/false(is horizontal); validate placement
    }
}

//TODO: format output(even spacing); print whole strings instead of each char in board
void printUI(char computerBoard[15][15], char playerBoard[15][15], int boardSize) {
    cout<<" ";
    for (int col = 1; col <= boardSize; col++)
    {
        cout<<col<<" ";
    }
    cout<<endl;
    
    for (int row = 0; row < boardSize; row++)
    {
        cout<<row + 1<<" ";
        for (int col = 0; col < boardSize; col++)
        {
            cout<<computerBoard[row][col]<<" ";
        }
        cout<<"| ";
        for (int col = 0; col < boardSize; col++)
        {
            cout<<playerBoard[row][col]<<" ";
        }
        cout<<endl;
    }
}