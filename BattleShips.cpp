#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace std;

const int SHIPS_COUNT = 10;
const int SHIP_SIZES[SHIPS_COUNT] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

void saveGameStateToFile(char[15][15], char[15][15], int, bool);

void loadGameFromFile(char[15][15], char[15][15], int&, bool&);

void startNewGame(char[15][15], char[15][15], int, bool, bool);

bool isValidPlacement(char[15][15], int, int, int, int, bool);

void placeShip(char[15][15], int, int, int, bool);

void automaticallyFillBoard(char[15][15], int);

bool checkLoss(char[15][15], int boardSize);

void printUI(char[15][15], char[15][15], int);

void printBoard(char[15][15], int);

void printStartingMenu();

void printDifficultyMenu();

void clearConsole();

void inputNumber(int&, const char*);

void inputBool(bool&, const char*);

bool stringEquals(const char*, const char*);

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

    while (choice != 1 && choice != 2) {
        inputNumber(choice, "choice");
    }
    
    clearConsole();

    char computerBoard[15][15];
    char playerBoard[15][15];
    
    int boardSize = 0;
    bool isPlayerTurn = true;

    // Load game from file
    if (choice == 2) {
        loadGameFromFile(computerBoard, playerBoard, boardSize, isPlayerTurn);

        if (boardSize == 0) {
            clearConsole();
            cout << "No save present. Starting new game..." << endl;
            choice = 1;
        }
    }

    // Start new game
    if (choice == 1) {
        printDifficultyMenu();
        
        int difficultyChoice = 0;
        while (difficultyChoice < 1 || difficultyChoice > 3) {
            inputNumber(difficultyChoice, "difficulty");
        }

        clearConsole();

        switch(difficultyChoice) {
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

        inputBool(manualPlayerInput, "input");

        clearConsole();
        
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                playerBoard[i][j] = '~';
                computerBoard[i][j] = '~';
            }
        }

        startNewGame(computerBoard, playerBoard, boardSize, manualPlayerInput, isPlayerTurn);
    }

    bool isGameOver = false;

    // Game loop
    while (!isGameOver) {
        clearConsole();

        printUI(computerBoard, playerBoard, boardSize);
        
        saveGameStateToFile(computerBoard, playerBoard, boardSize, isPlayerTurn);

        if (isPlayerTurn) {
            cout << "Your turn! Enter coordinates" << endl;
            int r, c;
            
            cout << "Row: ";
            inputNumber(r, "row");
            cout << "Col: ";
            inputNumber(c, "column");

            r--;
            c--;

            // Validation, if invalid retry
            if (r < 0 || r >= boardSize || c < 0 || c >= boardSize) {
                cout << "Invalid coordinates! Try again." << endl;

                Sleep(1500);

                continue;
            }
            if (computerBoard[r][c] == 'X' || computerBoard[r][c] == 'o') {
                cout << "You already shot there! Try again." << endl;

                Sleep(1500);

                continue;
            }

            // Shooting logic
            if (computerBoard[r][c] == '#') {
                computerBoard[r][c] = 'X';            
                if (checkLoss(computerBoard, boardSize)) {
                    cout << "\nCongratulations! You won!" << endl;
                    isGameOver = true;
                    break;
                }
                cout << "HIT! You get another turn." << endl;
                Sleep(1500);
                continue;
            }
            else {
                computerBoard[r][c] = 'o';
                cout << "Miss." << endl;
            }
            Sleep(1500);
        } 
        else {
            // Computer turn logic
            cout << "\n[COMPUTER TURN]" << endl;
            cout << "Computer is thinking..." << endl;

            Sleep(1500);

            bool validShot = false;
            bool isTargetShip = false;
            do {
                int r = rand() % boardSize;
                int c = rand() % boardSize;

                if (playerBoard[r][c] != 'X' && playerBoard[r][c] != 'o') {
                    validShot = true;
                    if (playerBoard[r][c] == '#') {
                        playerBoard[r][c] = 'X';
                        cout << "Computer HIT at " << r+1 << " " << c+1 << "!" << endl;
                        if (checkLoss(playerBoard, boardSize)) {
                            cout << "Game Over! Computer wins." << endl;
                            isGameOver = true;
                        }
                        // Computer keeps turn
                        isTargetShip = true;
                    } 
                    else {
                        playerBoard[r][c] = 'o';
                        cout << "Computer MISSED at " << r+1 << " " << c+1 << "." << endl;
                        Sleep(1500);
                    }
                }
            } while (!validShot);

            if (isTargetShip) {
                Sleep(1500);
                continue;
            }
        }
        
        if (!isGameOver) {
            isPlayerTurn = !isPlayerTurn;  
        }
    }
    remove("battleship_save.txt");
    system("pause");

    clearConsole();
    main();
}

void saveGameStateToFile(char computerBoard[15][15], char playerBoard[15][15], int boardSize, bool isPlayerTurn) {
    ofstream outFile("battleship_save.txt");

    if (outFile.is_open()) {
        // Game configuration
        outFile << boardSize << " " << isPlayerTurn << endl;

        // Computer board
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                outFile << computerBoard[i][j] << " ";
            }
            outFile << endl;
        }

        // Player board
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                outFile << playerBoard[i][j] << " ";
            }
            outFile << endl;
        }

        outFile.close();
    }
}

void loadGameFromFile(char computerBoard[15][15], char playerBoard[15][15], int& boardSize, bool& isPlayerTurn) {
    ifstream inFile("battleship_save.txt");

    if (inFile.is_open()) {
        // Game configuration
        inFile >> boardSize >> isPlayerTurn;

        // Computer board
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                inFile >> computerBoard[i][j];
            }
        }

        // Player board
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                inFile >> playerBoard[i][j];
            }
        }

        inFile.close();
    } 
    else {
        cout << "No save point found." << endl;
        boardSize = 0;
    }
}

bool isValidPlacement(char board[15][15], int boardSize, int shipLength, int row, int col, bool isHorizontal) {
    if (isHorizontal) {
        if (shipLength + col > boardSize) {
            return false;
        }
    } 
    else {
        if (shipLength + row > boardSize) {
            return false;
        }
    }

    for (int i = 0; i < shipLength; i++) 
    {
        if (isHorizontal) {
            if (board[row][col + i] != '~') {
                return false;
            }
        } 
        else {
            if (board[row + i][col] != '~') {
                return false;
            }
        }
    }

    return true;
}

void placeShip(char board[15][15], int row, int col, int shipLength, bool isHorizontal) {
    for (int i = 0; i < shipLength; i++) {
        if (isHorizontal) {
            board[row][col + i] = '#';
        } 
        else {
            board[row + i][col] = '#';
        }
    }
}

void automaticallyFillBoard(char board[15][15], int boardSize) {
    for (int i = 0; i < SHIPS_COUNT; i++) {
        int shipLength = SHIP_SIZES[i];
        bool isPlaced = false;

        // Randomly generate a tile until the ship placement is valid
        while (!isPlaced) {
            int row = rand() % boardSize;
            int col = rand() % boardSize;
            bool isHorizontal = rand() % 2;

            if (isValidPlacement(board, boardSize, shipLength, row, col, isHorizontal)) {
                placeShip(board, row, col, shipLength, isHorizontal);
                isPlaced = true;
            }
        }
    }
}

bool checkLoss(char board[15][15], int boardSize) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == '#') {
                // Ship found
                return false;
            }
        }
    }
    // Game end
    return true;
}

void startNewGame(char computerBoard[15][15], char playerBoard[15][15], int boardSize, bool manualInput, bool isPlayerTurn) {
    isPlayerTurn = rand() % 2;

    // Automatically generate computer board
    automaticallyFillBoard(computerBoard, boardSize);

    if (!manualInput) {
        // Automatically generate user board
        automaticallyFillBoard(playerBoard, boardSize);
    } 
    else {
        // Manual input
        for (int i = 0; i < SHIPS_COUNT; i++) {
            int shipSize = SHIP_SIZES[i];

            int row, col;
            bool isHorizontal;
            do {
                printBoard(playerBoard, boardSize);

                cout << "Place a " << shipSize << " square long ship" << endl;
                
                cout << "Enter row (1 - " << boardSize << "): ";
                while (row < 1 && row > boardSize) {
                    inputNumber(row, "row");
                }

                cout << "Enter column (1 - " << boardSize << "): ";
                while (col < 1 && col > boardSize) {
                    inputNumber(col, "column");
                }

                if (shipSize != 1) {
                    cout << "Will the ship be horizontal? (Yes/No | True/False | 1/0):";
                    inputBool(isHorizontal, "horizontality");
                }                

                row--;
                col--;

                clearConsole();
            } while (!isValidPlacement(playerBoard, boardSize, shipSize, row, col, isHorizontal));

            placeShip(playerBoard, row, col, shipSize, isHorizontal);
        }
    }
}

void printUI(char computerBoard[15][15], char playerBoard[15][15], int boardSize) {
    cout << "   ";
    for (int col = 1; col <= boardSize; col++) {
        if (col < 10) {
            cout << " ";
        }
        cout << col << " ";
    }

    cout << "   ";
    for (int col = 1; col <= boardSize; col++) {
        if (col < 10){
            cout << " ";            
        }
        cout << col << " ";
    }
    
    cout << endl;
    
    for (int row = 0; row < boardSize; row++) {
        int rowNumber = row + 1;

        if (rowNumber < 10) {
            cout << " ";
        }
        cout << rowNumber << " ";
        
        // Computer Board (Ships which are not hit are not shown)
        for (int col = 0; col < boardSize; col++) {
            if (boardSize > 9) {
                cout << " ";
            }
            char tile = computerBoard[row][col];
            if (tile == '#') {
                cout << "~ ";
            } else {
                cout << tile << " ";
            }
        }

        cout << " | ";
        
        // Player Board
        for (int col = 0; col < boardSize; col++){
            if (boardSize > 9) {
                cout << " ";
            }
            cout << playerBoard[row][col] << " ";
        }
        cout << endl;
    }
}

void printBoard(char board[15][15], int boardSize) {
    cout << "   ";
    for (int col = 1; col <= boardSize; col++) {
        if (col < 10) {
            cout << " ";
        }
        cout << col << " ";
    }

    cout << endl;
    
    for (int row = 0; row < boardSize; row++) {
        int rowNumber = row + 1;

        if (rowNumber < 10) {
            cout << " ";
        }
        cout << rowNumber << " ";

        for (int col = 0; col < boardSize; col++) {
            if (boardSize > 9) {
                cout << " ";
            }
            cout << board[row][col] << " ";
        }
        
        cout << endl;
    }
}

void printStartingMenu() {
    cout << "=== BATTLESHIPS ===" << endl;
    cout << "1. New game" << endl;
    cout << "2. Load game" << endl;
    cout << "Choose: ";
}

void printDifficultyMenu() {
    cout << "=== Choose Difficulty ===" << endl;
    cout << "1. Calm Waters (10x10)" << endl;
    cout << "2. Rough Seas (12x12)" << endl;
    cout << "3. Storm of Steel (15x15)" << endl;
    cout << "Choose: ";
}

void clearConsole() {
    system("cls");
}

void inputNumber(int& number, const char* input) {
    while (true) {
        cin >> number;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid " << input << ", try again: ";
        } 
        else {
            break;
        }
    }
}

void inputBool(bool& boolean, const char* input) {
    char buffer[50];
    while (true) {
        cin >> buffer;

        // Convert to lowercase
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                buffer[i] += 32;
            }
        }

        // Check for True values
        if (stringEquals(buffer, "yes") || stringEquals(buffer, "true") || stringEquals(buffer, "1")) {
            boolean = true;
            return;
        }
        // Check for False values
        if (stringEquals(buffer, "no") || stringEquals(buffer, "false") || stringEquals(buffer, "0")) {
            boolean = false;
            return;
        }

        cout << "Invalid " << input << "! Please enter Yes/No, True/False, or 1/0: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

bool stringEquals(const char* firstString, const char* secondString) {
    int i = 0;
    while (firstString[i] != '\0' && secondString[i] != '\0') {
        if (firstString[i] != secondString[i]) {
            return false;
        }
        i++;
    }
    return firstString[i] == '\0' && secondString[i] == '\0';
}