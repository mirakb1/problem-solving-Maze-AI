#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
using namespace std;

bool findPosition(char** cells, int rows, int columns, char aim, int& row, int& col) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (cells[i][j] == aim) {
				row = i;
				col = j;
				return true;
			}
		}
	}
	return false;
}

int manhattanDistance(int initialRow, int initialColumn, int goalRow, int goalColumn) {
	int distance = abs(initialRow - goalRow) + abs(initialColumn - goalColumn);
	return distance;
}

bool transition(int currentRow, int currentColumn, int nextRow, int nextColumn,
	int** horizontalBorders, int** verticalBorders) {
	//هاد لليمين
	if (nextRow == currentRow && nextColumn == currentColumn + 1) {
		return horizontalBorders[currentRow][currentColumn] == 0;
	}
	//شمال
	if (nextRow == currentRow && nextColumn == currentColumn - 1) {
		return horizontalBorders[currentRow][currentColumn - 1] == 0;
	}
	// تحت
	if (nextRow == currentRow + 1 && nextColumn == currentColumn) {
		return verticalBorders[currentColumn][currentRow] == 0;
	}
	//فوق
	if (nextRow == currentRow - 1 && nextColumn == currentColumn) {
		return verticalBorders[currentColumn][currentRow - 1] == 0;
	}

	return false;
}

bool isVisited(char state, char path[], int pathLength) {
	for (int i = 0; i < pathLength; i++) {
		if (path[i] == state) {
			return true;
		}
	}
	return false;
}

class node {
public:
	char name;
	int row;
	int column;
	int heuristic;
};

int findBestNodeIndex(node openList[], int openCount) {
	int bestIndex = 0;

	for (int i = 1; i < openCount; i++) {
		if (openList[i].heuristic < openList[bestIndex].heuristic ||
			(openList[i].heuristic == openList[bestIndex].heuristic &&
				openList[i].name < openList[bestIndex].name)) {
			bestIndex = i;
		}
	}

	return bestIndex;
}

int main() {
	string Filename;
	cout << "enter name of text file: ";
	cin >> Filename;
	ifstream inFile(Filename);
	if (!inFile) {
		cout << "error" << endl;
		return 1;
	}

	string line;
	int rows = 0, columns = 0;

	while (getline(inFile, line)) {
		if (line.empty()) continue;

		stringstream linestream(line);
		string firstword;
		linestream >> firstword;

		if (isalpha(firstword[0])) {
			rows++;
			int count = 1;
			string word;

			while (linestream >> word) count++;

			if (rows == 1) columns = count;
		}
		else break;
	}

	cout << "Rows = " << rows << endl;
	cout << "Columns = " << columns << endl;

	inFile.close();

	char** cells = new char* [rows];
	for (int i = 0; i < rows; i++) {
		cells[i] = new char[columns];
	}

	inFile.open(Filename);
	if (!inFile) {
		cout << "error" << endl;
		return 1;
	}

	int currentRow = 0;

	while (getline(inFile, line)) {
		if (line.empty()) continue;

		stringstream linestream(line);
		string firstword;
		linestream >> firstword;

		if (isalpha(firstword[0])) {
			cells[currentRow][0] = firstword[0];

			for (int j = 1; j < columns; j++) {
				string word;
				linestream >> word;
				cells[currentRow][j] = word[0];
			}

			currentRow++;
		}
		else break;
	}

	cout << "----Cells----" << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << cells[i][j] << " ";
		}
		cout << endl;
	}

	inFile.close();

	int** horizontalBorders = new int* [rows];
	for (int i = 0; i < rows; i++) {
		horizontalBorders[i] = new int[columns - 1];
	}

	int** verticalBorders = new int* [columns];
	for (int i = 0; i < columns; i++) {
		verticalBorders[i] = new int[rows - 1];
	}

	inFile.open(Filename);
	if (!inFile) {
		cout << "error" << endl;
		return 1;
	}

	while (getline(inFile, line)) {
		if (line.empty()) continue;

		stringstream linestream(line);
		string firstword;
		linestream >> firstword;

		if (!isalpha(firstword[0])) {
			stringstream borderstream(line);
			for (int j = 0; j < columns - 1; j++) {
				borderstream >> horizontalBorders[0][j];
			}
			break;
		}
	}

	for (int i = 1; i < rows; i++) {
		getline(inFile, line);
		while (line.empty()) getline(inFile, line);

		stringstream borderstream(line);
		for (int j = 0; j < columns - 1; j++) {
			borderstream >> horizontalBorders[i][j];
		}
	}

	for (int i = 0; i < columns; i++) {
		getline(inFile, line);
		while (line.empty()) getline(inFile, line);

		stringstream borderstream(line);
		for (int j = 0; j < rows - 1; j++) {
			borderstream >> verticalBorders[i][j];
		}
	}

	inFile.close();

	cout << "---Horizontal Borders---" << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns - 1; j++) {
			cout << horizontalBorders[i][j] << " ";
		}
		cout << endl;
	}

	cout << "-Vertical Borders-" << endl;
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows - 1; j++) {
			cout << verticalBorders[i][j] << " ";
		}
		cout << endl;
	}

	char initialState, goalState;
	cout << "enter initial state: ";
	cin >> initialState;
	cout << "enter goal state: ";
	cin >> goalState;

	int initialRow, initialColumn, goalRow, goalCol;

	bool foundinitial = findPosition(cells, rows, columns, initialState, initialRow, initialColumn);
	bool foundGoal = findPosition(cells, rows, columns, goalState, goalRow, goalCol);

	if (!foundinitial || !foundGoal) {
		cout << "invalid state\n";
	}
	else {
		cout << "\ninitial position: (" << initialRow << ", " << initialColumn << ")" << endl;
		cout << "goal position: (" << goalRow << ", " << goalCol << ")" << endl;

		int heuristic = manhattanDistance(initialRow, initialColumn, goalRow, goalCol);

		node startNode;
		startNode.name = initialState;
		startNode.row = initialRow;
		startNode.column = initialColumn;
		startNode.heuristic = heuristic;

		node openList[100];
		int openCount = 0;

		char visited[100];
		int visitedCount = 0;

		int parentRow[100][100];
		int parentCol[100][100];

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				parentRow[i][j] = -1;
				parentCol[i][j] = -1;
			}
		}

		openList[openCount] = startNode;
		openCount++;

		int Nodesexpanded = 0;
		bool foundGoalState = false;
		node goalNode;

		while (openCount > 0) {

			int bestIndex = findBestNodeIndex(openList, openCount);
			node currentNode = openList[bestIndex];

			for (int i = bestIndex; i < openCount - 1; i++) {
				openList[i] = openList[i + 1];
			}
			openCount--;

			if (isVisited(currentNode.name, visited, visitedCount)) {
				continue;
			}

			visited[visitedCount] = currentNode.name;
			visitedCount++;

			if (currentNode.name == goalState) {
				foundGoalState = true;
				goalNode = currentNode;
				break;
			}

			Nodesexpanded++;

			//هاد لليمين
			if (currentNode.column + 1 < columns) {
				if (transition(currentNode.row, currentNode.column, currentNode.row, currentNode.column + 1, horizontalBorders, verticalBorders)) {
					char nextName = cells[currentNode.row][currentNode.column + 1];

					if (!isVisited(nextName, visited, visitedCount)) {
						node rightNeighbor;
						rightNeighbor.name = nextName;
						rightNeighbor.row = currentNode.row;
						rightNeighbor.column = currentNode.column + 1;
						rightNeighbor.heuristic = manhattanDistance(rightNeighbor.row, rightNeighbor.column, goalRow, goalCol);

						openList[openCount] = rightNeighbor;
						openCount++;

						if (parentRow[rightNeighbor.row][rightNeighbor.column] == -1) {
							parentRow[rightNeighbor.row][rightNeighbor.column] = currentNode.row;
							parentCol[rightNeighbor.row][rightNeighbor.column] = currentNode.column;
						}
					}
				}
			}
			//هاد لليشار 
			if (currentNode.column - 1 >= 0) {
				if (transition(currentNode.row, currentNode.column, currentNode.row, currentNode.column - 1, horizontalBorders, verticalBorders)) {
					char nextName = cells[currentNode.row][currentNode.column - 1];

					if (!isVisited(nextName, visited, visitedCount)) {
						node leftNeighbor;
						leftNeighbor.name = nextName;
						leftNeighbor.row = currentNode.row;
						leftNeighbor.column = currentNode.column - 1;
						leftNeighbor.heuristic = manhattanDistance(leftNeighbor.row, leftNeighbor.column, goalRow, goalCol);

						openList[openCount] = leftNeighbor;
						openCount++;

						if (parentRow[leftNeighbor.row][leftNeighbor.column] == -1) {
							parentRow[leftNeighbor.row][leftNeighbor.column] = currentNode.row;
							parentCol[leftNeighbor.row][leftNeighbor.column] = currentNode.column;
						}
					}
				}
			}
			//هاد تحت
			if (currentNode.row + 1 < rows) {
				if (transition(currentNode.row, currentNode.column, currentNode.row + 1, currentNode.column, horizontalBorders, verticalBorders)) {
					char nextName = cells[currentNode.row + 1][currentNode.column];

					if (!isVisited(nextName, visited, visitedCount)) {
						node downNeighbor;
						downNeighbor.name = nextName;
						downNeighbor.row = currentNode.row + 1;
						downNeighbor.column = currentNode.column;
						downNeighbor.heuristic = manhattanDistance(downNeighbor.row, downNeighbor.column, goalRow, goalCol);

						openList[openCount] = downNeighbor;
						openCount++;

						if (parentRow[downNeighbor.row][downNeighbor.column] == -1) {
							parentRow[downNeighbor.row][downNeighbor.column] = currentNode.row;
							parentCol[downNeighbor.row][downNeighbor.column] = currentNode.column;
						}
					}
				}
			}
			//وهاد فوق
			if (currentNode.row - 1 >= 0) {
				if (transition(currentNode.row, currentNode.column, currentNode.row - 1, currentNode.column, horizontalBorders, verticalBorders)) {
					char nextName = cells[currentNode.row - 1][currentNode.column];

					if (!isVisited(nextName, visited, visitedCount)) {
						node upNeighbor;
						upNeighbor.name = nextName;
						upNeighbor.row = currentNode.row - 1;
						upNeighbor.column = currentNode.column;
						upNeighbor.heuristic = manhattanDistance(upNeighbor.row, upNeighbor.column, goalRow, goalCol);

						openList[openCount] = upNeighbor;
						openCount++;

						if (parentRow[upNeighbor.row][upNeighbor.column] == -1) {
							parentRow[upNeighbor.row][upNeighbor.column] = currentNode.row;
							parentCol[upNeighbor.row][upNeighbor.column] = currentNode.column;
						}
					}
				}
			}
		}

		if (foundGoalState) {
			char path[100];
			int pathLength = 0;

			int r = goalNode.row;
			int c = goalNode.column;

			while (r != -1 && c != -1) {
				path[pathLength] = cells[r][c];
				pathLength++;

				int pr = parentRow[r][c];
				int pc = parentCol[r][c];

				r = pr;
				c = pc;
			}

			cout << "The solution is: ";
			for (int i = pathLength - 1; i >= 0; i--) {
				cout << path[i];
			}
			cout << endl;

			cout << "Number of nodes expanded: " << Nodesexpanded << endl;
		}
		else {
			cout << "No path found" << endl;
			cout << "Number of nodes expanded: " << Nodesexpanded << endl;
		}
	}

	for (int i = 0; i < rows; i++) delete[] cells[i];
	delete[] cells;

	for (int i = 0; i < rows; i++) delete[] horizontalBorders[i];
	delete[] horizontalBorders;

	for (int i = 0; i < columns; i++) delete[] verticalBorders[i];
	delete[] verticalBorders;
}