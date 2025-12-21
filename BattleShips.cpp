#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace std;

const int SHIPS_COUNT = 10;
const int SHIP_SIZES[SHIPS_COUNT] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

void saveGameStateToFile(char[15][15], char[15][15], int, bool);

void loadGameFromFile(char[15][15], char[15][15], int, bool);

void startNewGame(char[15][15], char[15][15], int, bool, bool);

bool isValidPlacement(char[15][15], int, int, int, int, bool);

void placeShip(char[15][15], int, int, int, bool);

void automaticallyFillBoard(char[15][15], int);

void printUI(char[15][15], char[15][15], int);

void printStartingMenu();

void printDifficultyMenu();

int main() {
    srand(time(0));
    //1. Choose to load/start new game, load - skip to 5
    //2. Choose difficulty: 
    //      - Calm Waters (дъска 10x10)
    //      - Rough Seas (дъска 12x12)
    //      - Storm of Steel (дъска 15x15)
    //3. Fill computer board
    //4. Fill player board
    //5. Play out game

    printStartingMenu();

    int choice = 0;
    cin >> choice;

    char computerBoard[15][15];
    char playerBoard[15][15];
    
    int boardSize = 0;
    bool isPlayerTurn;

    //TODO: load or start new game depending on choice; automatic player input
    if (choice == 2) 
    {
        //TODO: file reading
        choice = 1;
    }

    if (choice == 1) 
    {
        printDifficultyMenu();
        
        int difficultyChoice = 0;

        cin >> difficultyChoice;

        switch(difficultyChoice) 
        {
            case 1: boardSize = 10; 
                break;
            case 2: boardSize = 12; 
                break;
            case 3: boardSize = 15; 
                break;
            default: boardSize = 10; 
                break;
        }

        bool manualPlayerInput = false;

        cout << "Would you like manual input? (1 - Yes, 0 - No): ";

        cin >> manualPlayerInput;
        
        for(int i = 0; i < 15; i++) 
        {
            for(int j = 0; j < 15; j++) 
            {
                playerBoard[i][j] = '~';
                computerBoard[i][j] = '~';
            }
        }

        startNewGame(computerBoard, playerBoard, boardSize, manualPlayerInput, isPlayerTurn);
    }

    bool isGameOver = false;

    //TODO: game cycle; playTurn(..., isPlayerTurn); at the end of each turn check if the game is ended; when end, restart

    //retry input if invalid
    while (!isGameOver) 
    {
        printUI(computerBoard, playerBoard, boardSize);

        int n = 0;
        cin >> n;
    }
}

bool isValidPlacement(char board[15][15], int boardSize, int shipLength, int row, int col, bool isHorizontal) {
    if (isHorizontal) 
    {
        if (shipLength + col > boardSize) 
        {
            return false;
        }
    } 
    else 
    {
        if (shipLength + row > boardSize) 
        {
            return false;
        }
    }

    for (int i = 0; i < shipLength; i++) 
    {
        if (isHorizontal) 
        {
            if (board[row][col + i] != '~') {
                return false;
            }
        } 
        else 
        {
            if (board[row + i][col] != '~') 
            {
                return false;
            }
        }
    }

    return true;
}

void placeShip(char board[15][15], int row, int col, int shipLength, bool isHorizontal) {
    for (int i = 0; i < shipLength; i++) 
    {
        if (isHorizontal) 
        {
            board[row][col + i] = '#';
        } 
        else 
        {
            board[row + i][col] = '#';
        }
    }
}

void automaticallyFillBoard(char board[15][15], int boardSize) {
    //TODO: depending on board size place: 4 - 1 tile ships, 3 - 2 tile ships, 2 - 3 tile ships, 1 - 4 tile ship

    //TODO: generate a random tile in the board and check if valid

    for (int i = 0; i < SHIPS_COUNT; i++) 
    {
        int shipLength = SHIP_SIZES[i];
        bool isPlaced = false;

        while (!isPlaced) 
        {
            int row = rand() % boardSize;
            int col = rand() % boardSize;
            bool horizontal = rand() % 2;

            if (isValidPlacement(board, boardSize, shipLength, row, col, horizontal)) 
            {
                placeShip(board, row, col, shipLength, horizontal);
                isPlaced = true;
            }
        }
    }
}

void startNewGame(char computerBoard[15][15], char playerBoard[15][15], int boardSize, bool manualInput, bool isPlayerTurn) {
    isPlayerTurn = rand() % 2;

    automaticallyFillBoard(computerBoard, boardSize);

    if (!manualInput) 
    {
        automaticallyFillBoard(playerBoard, boardSize);
    } 
    else 
    {
        //TODO: manual ship input, example: 0(first coord) 2(second coord) 0/1/true/false(is horizontal); validate placement
    }
}

//TODO: format output(even spacing); print whole strings instead of each char in board
void printUI(char computerBoard[15][15], char playerBoard[15][15], int boardSize) {
    cout << "  ";
    for (int col = 1; col <= boardSize; col++) 
    {
        if (col < 10) 
        {
            cout << " " << col << " ";
        } 
        else 
        {
            cout << col << " ";
        }
    }

    cout << "     ";
    for (int col = 0; col < boardSize; col++) 
    {
        if (col < 10)
        {
            cout << " " << col << " ";            
        }
        else
        {
            cout << col << " ";
        }
    }
    
    cout << endl;
    
    for (int row = 0; row < boardSize; row++) 
    {
        int rowNumber = row + 1;
        if (rowNumber < 10) 
        {
            cout << " " << rowNumber << " ";
        }
        else 
        {
            cout << rowNumber << " ";
        }
        
        for (int col = 0; col < boardSize; col++)
        {
            cout << computerBoard[row][col] << " ";
        }
        cout<<"| ";
        for (int col = 0; col < boardSize; col++)
        {
            cout << playerBoard[row][col] << " ";
        }
        cout<<endl;
    }
}

void printStartingMenu() {
    cout << "=== BATTLESHIPS ===" << endl;
    cout << "1. New game" << endl;
    cout << "2. Load game" << endl;
    cout << "Choose: ";
}

void printDifficultyMenu() {
    cout << "\nChoose difficulty:" << endl;
    cout << "1. Calm Waters (10x10)" << endl;
    cout << "2. Rough Seas (12x12)" << endl;
    cout << "3. Storm of Steel (15x15)" << endl;
    cout << "Choose: ";
}