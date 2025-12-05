#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace std;

void saveGameStateToFile(char[15][15], char[15][15], int, bool);

void loadGameFromFile();

void startNewGame(char[15][15], char[15][15], int, bool, bool);

void printUI();

int main() {
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

    bool manualPlayerInput = true;

    startNewGame(computerBoard, playerBoard, boardSize, manualPlayerInput, isPlayerTurn);

    printUI();
}

void isValidPlacement(char board[15][15], int shipLength, bool isHorizontal) {

}

void automaticallyFillBoard(char board[15][15], int boardSize) {
    //TODO: depending on board size place: 4 - 1 tile ships, 3 - 2 tile ships, 2 - 3 tile ships, 1 - 4 tile ship

    //TODO: generate a random tile in the board and check if valid
}

void startNewGame(char computerBoard[15][15], char playerBoard[15][15], int boardSize, bool manualInput, bool isPlayerTurn) {
    automaticallyFillBoard(computerBoard, boardSize);

    if (!manualInput) {
        automaticallyFillBoard(playerBoard, boardSize);
    }
}