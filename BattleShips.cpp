#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace std;

const int SHIPS_COUNT = 10;
const int MAX_BOARD_SIZE = 15;
const int SHIP_SIZES[SHIPS_COUNT] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

const int CALM_WATERS_SIZE = 10;
const int ROUGH_SEAS_SIZE = 12;
const int STORM_OF_STEEL_SIZE = 15;

const char EMPTY_TILE = '~';
const char SHIP_TILE = '#';
const char HIT_TILE = 'X';
const char MISS_TILE = 'o';

const int SLEEP_MILLISECONDS = 1500;

void initializeGame(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int& boardSize, bool& isPlayerTurn);

void playOutGame(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, bool isPlayerTurn);

bool handlePlayerTurn(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, bool& isGameOver);

bool handleComputerTurn(char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, bool& isGameOver);

void saveGameStateToFile(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, const bool isPlayerTurn);

void loadGameFromFile(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int& boardSize, bool& isPlayerTurn);

void startNewGame(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, const bool manualInput, bool isPlayerTurn);

bool isValidPlacement(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, const int shipLength, const int row, const int col, const bool isHorizontal);

void placeShip(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int row, const int col, const int shipLength, const bool isHorizontal);

void automaticallyFillBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize);

bool checkLoss(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize);

void printUI(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize);

void printCenteredText(const char* text, const int width);

void printSpaces(const int count);

int textLength(const char* text);

void printBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize);

void printBoardRow(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int row, const int boardSize, const bool areShipsHidden);

void printBoardTopIndexes(const int boardSize);

void printStartingMenu();

void printDifficultyMenu();

void clearConsole();

void inputNumber(int& number, const char* inputName);

void inputBool(bool& boolean, const char* inputName);

int getNumberWithBoundries(const char* inputMessage, const char* inputName, const int boundryStart, const int boundryEnd);

int getDifficultyInput();

bool stringEquals(const char* firstString, const char* secondString);

int main() {
	srand(time(0));

	char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

	int boardSize = 0;
	bool isPlayerTurn = true;

	initializeGame(computerBoard, playerBoard, boardSize, isPlayerTurn);

	playOutGame(computerBoard, playerBoard, boardSize, isPlayerTurn);
}

void initializeGame(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int& boardSize, bool& isPlayerTurn) {
	//1. Choose to load/start new game, load - skip to 5
	//2. Choose difficulty: 
	//      - Calm Waters (дъска 10x10)
	//      - Rough Seas (дъска 12x12)
	//      - Storm of Steel (дъска 15x15)
	//3. Fill computer board
	//4. Fill player board
	//5. Play out game
	printStartingMenu();
	int choice = getNumberWithBoundries("Choice: ", "choice", 1, 2);
	clearConsole();

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
		boardSize = getDifficultyInput();

		bool manualPlayerInput = false;
		cout << "Would you like manual input? (1 - Yes, 0 - No): ";
		inputBool(manualPlayerInput, "input");
		clearConsole();

		for (int i = 0; i < MAX_BOARD_SIZE; i++) {
			for (int j = 0; j < MAX_BOARD_SIZE; j++) {
				playerBoard[i][j] = EMPTY_TILE;
				computerBoard[i][j] = EMPTY_TILE;
			}
		}
		startNewGame(computerBoard, playerBoard, boardSize, manualPlayerInput, isPlayerTurn);
	}
}

void playOutGame(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, bool isPlayerTurn) {
	bool isGameOver = false;

	while (!isGameOver) {
		clearConsole();

		printUI(computerBoard, playerBoard, boardSize);

		saveGameStateToFile(computerBoard, playerBoard, boardSize, isPlayerTurn);

		if (isPlayerTurn) {
			bool keepTurn = handlePlayerTurn(computerBoard, boardSize, isGameOver);
			if (keepTurn) {
				continue;
			}
		}
		else {
			bool keepTurn = handleComputerTurn(playerBoard, boardSize, isGameOver);
			if (keepTurn) {
				continue;
			}
		}

		if (!isGameOver) {
			isPlayerTurn = !isPlayerTurn;
		}
	}
	remove("battleship_save.txt");
	system("pause");
}

bool handlePlayerTurn(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, bool& isGameOver) {
	cout << "Your turn! Enter coordinates" << endl;
	int row = getNumberWithBoundries("Row: ", "row", 1, boardSize);
	int col = getNumberWithBoundries("Col: ", "col", 1, boardSize);
	row--; col--;

	// Validation, if invalid retry
	char computerTile = computerBoard[row][col];
	if (computerTile == HIT_TILE || computerTile == MISS_TILE) {
		cout << "You already shot there! Try again." << endl;
		Sleep(SLEEP_MILLISECONDS);
		return true;
	}

	// Shooting logic
	if (computerTile == SHIP_TILE) {
		computerBoard[row][col] = HIT_TILE;
		if (checkLoss(computerBoard, boardSize)) {
			cout << "\nCongratulations! You won!" << endl;
			isGameOver = true;
			return false;
		}
		cout << "HIT! You get another turn." << endl;
		Sleep(SLEEP_MILLISECONDS);
		return true;
	}
	computerBoard[row][col] = MISS_TILE;
	cout << "Miss." << endl;
	Sleep(SLEEP_MILLISECONDS);
	return false;
}

bool handleComputerTurn(char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, bool& isGameOver) {
	cout << "\n[COMPUTER TURN]" << endl;
	cout << "Computer is thinking..." << endl;

	Sleep(SLEEP_MILLISECONDS);

	bool validShot = false, isTargetShip = false;
	do {
		int row = rand() % boardSize;
		int col = rand() % boardSize;

		char playerTile = playerBoard[row][col];
		if (playerTile != HIT_TILE && playerTile != MISS_TILE) {
			validShot = true;
			if (playerTile == SHIP_TILE) {
				playerBoard[row][col] = HIT_TILE;
				cout << "Computer HIT at " << row + 1 << " " << col + 1 << "!" << endl;
				if (checkLoss(playerBoard, boardSize)) {
					cout << "Game Over! Computer wins." << endl;
					isGameOver = true;
				}
				isTargetShip = true;
				continue;
			}
			playerBoard[row][col] = MISS_TILE;
			cout << "Computer MISSED at " << row + 1 << " " << col + 1 << "." << endl;
		}
	} while (!validShot);
	Sleep(SLEEP_MILLISECONDS);
	return isTargetShip;
}

void saveGameStateToFile(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, const bool isPlayerTurn) {
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

void loadGameFromFile(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int& boardSize, bool& isPlayerTurn) {
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
		return;
	}
	boardSize = 0;
}

bool isValidPlacement(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, const int shipLength, const int row, const int col, const bool isHorizontal) {
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
			if (board[row][col + i] != EMPTY_TILE) {
				return false;
			}
		}
		else {
			if (board[row + i][col] != EMPTY_TILE) {
				return false;
			}
		}
	}

	return true;
}

void placeShip(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int row, const int col, const int shipLength, const bool isHorizontal) {
	for (int i = 0; i < shipLength; i++) {
		if (isHorizontal) {
			board[row][col + i] = SHIP_TILE;
			continue;
		}
		board[row + i][col] = SHIP_TILE;
	}
}

void automaticallyFillBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize) {
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

bool checkLoss(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize) {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (board[i][j] == SHIP_TILE) {
				return false;
			}
		}
	}
	return true;
}

void startNewGame(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize, const bool manualInput, bool isPlayerTurn) {
	isPlayerTurn = rand() % 2;

	// Automatically generate computer board
	automaticallyFillBoard(computerBoard, boardSize);

	if (!manualInput) {
		// Automatically generate user board
		automaticallyFillBoard(playerBoard, boardSize);
		return;
	}
	// Manual input
	for (int i = 0; i < SHIPS_COUNT; i++) {
		int shipSize = SHIP_SIZES[i];

		int row = 0, col = 0;
		bool isHorizontal = false;
		do {
			printBoard(playerBoard, boardSize);
			cout << "Place a " << shipSize << " square long ship" << endl;
			row = getNumberWithBoundries("Enter row: ", "row", 1, boardSize);
			col = getNumberWithBoundries("Enter column: ", "column", 1, boardSize);
			row--; col--;

			if (shipSize != 1) {
				cout << "Will the ship be horizontal? (Yes/No | True/False | 1/0): ";
				inputBool(isHorizontal, "horizontality");
			}
			clearConsole();
		} while (!isValidPlacement(playerBoard, boardSize, shipSize, row, col, isHorizontal));

		placeShip(playerBoard, row, col, shipSize, isHorizontal);
	}
}

void printUI(char computerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char playerBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize) {
	int cellWidth = 3, rowHeaderWidth = 3;
	int totalBoardWidth = rowHeaderWidth + (boardSize * cellWidth);

	printCenteredText("Computer", totalBoardWidth);
	cout << " ";
	printCenteredText("Player", totalBoardWidth);
	cout << endl << endl;

	// Computer board indexes
	printBoardTopIndexes(boardSize);

	// Player board indexes
	printBoardTopIndexes(boardSize);

	cout << endl;

	for (int row = 0; row < boardSize; row++) {
		int rowNumber = row + 1;

		if (rowNumber < 10) {
			cout << " ";
		}
		cout << rowNumber << " ";

		// Computer Board (Ships which are not hit are not shown)
		printBoardRow(computerBoard, row, boardSize, true);
		cout << " | ";
		printBoardRow(playerBoard, row, boardSize, false);
		cout << endl;
	}
}

void printCenteredText(const char* text, const int width) {
	int len = textLength(text);

	if (len >= width) {
		cout << text;
		return;
	}

	int leftPadding = (width - len) / 2;
	int rightPadding = width - leftPadding - len;

	printSpaces(leftPadding);
	cout << text;
	printSpaces(rightPadding);
}

void printSpaces(const int count) {
	for (int i = 0; i < count; i++) {
		cout << ' ';
	}
}

int textLength(const char* text) {
	int len = 0;
	while (text[len] != '\0') {
		len++;
	}
	return len;
}

void printBoardTopIndexes(const int boardSize) {
	cout << "   ";
	for (int col = 1; col <= boardSize; col++) {
		if (col < 10) {
			cout << " ";
		}
		cout << col << " ";
	}
}

void printBoardRow(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int row, const int boardSize, const bool areShipsHidden) {
	for (int col = 0; col < boardSize; col++) {
		if (boardSize > 9) {
			cout << " ";
		}
		char tile = board[row][col];
		if (tile == SHIP_TILE && areShipsHidden) {
			cout << EMPTY_TILE << " ";
		}
		else {
			cout << tile << " ";
		}
	}
}

void printBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int boardSize) {
	printBoardTopIndexes(boardSize);

	cout << endl;

	for (int row = 0; row < boardSize; row++) {
		int rowNumber = row + 1;

		if (rowNumber < 10) {
			cout << " ";
		}
		cout << rowNumber << " ";

		printBoardRow(board, row, boardSize, false);

		cout << endl;
	}
}

void printStartingMenu() {
	cout << "=== BATTLESHIPS ===" << endl;
	cout << "1. New game" << endl;
	cout << "2. Load game" << endl;
}

void printDifficultyMenu() {
	cout << "=== Choose Difficulty ===" << endl;
	cout << "1. Calm Waters (" << CALM_WATERS_SIZE << "x" << CALM_WATERS_SIZE << ")" << endl;
	cout << "2. Rough Seas (" << ROUGH_SEAS_SIZE << "x" << ROUGH_SEAS_SIZE << ")" << endl;
	cout << "3. Storm of Steel (" << STORM_OF_STEEL_SIZE << "x" << STORM_OF_STEEL_SIZE << ")" << endl;
}

void clearConsole() {
	system("cls");
}

void inputNumber(int& number, const char* inputName) {
	while (true) {
		cin >> number;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Invalid " << inputName << ", try again: ";
		}
		else {
			cin.ignore(10000, '\n');
			break;
		}
	}
}

int getNumberWithBoundries(const char* inputMessage, const char* inputName, const int boundryStart, const int boundryEnd) {
	int number = 0;
	do {
		cout << inputMessage;
		inputNumber(number, inputName);
	} while (number < boundryStart || number > boundryEnd);

	return number;
}

int getDifficultyInput() {
	int difficultyChoice = getNumberWithBoundries("Choose Difficulty: ", "difficulty", 1, 3);

	int boardSize = 0;
	switch (difficultyChoice) {
	case 1: boardSize = CALM_WATERS_SIZE;
		break;
	case 2: boardSize = ROUGH_SEAS_SIZE;
		break;
	case 3: boardSize = STORM_OF_STEEL_SIZE;
		break;
	default: boardSize = CALM_WATERS_SIZE;
		break;
	}

	clearConsole();

	return boardSize;
}

void inputBool(bool& boolean, const char* inputName) {
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

		cout << "Invalid " << inputName << "! Please enter Yes/No, True/False, or 1/0: ";
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