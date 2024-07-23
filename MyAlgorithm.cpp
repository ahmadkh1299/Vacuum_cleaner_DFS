#include "MyAlgorithm.h"

MyAlgorithm::MyAlgorithm()
        : maxSteps(0), wallsSensor(nullptr), dirtSensor(nullptr), batteryMeter(nullptr), currentRow(0), currentCol(0), isInitialized(false) {}

void MyAlgorithm::setMaxSteps(std::size_t maxSteps) {
    this->maxSteps = maxSteps;
}

void MyAlgorithm::setWallsSensor(const WallsSensor& sensor) {
    this->wallsSensor = &sensor;
}

void MyAlgorithm::setDirtSensor(const DirtSensor& sensor) {
    this->dirtSensor = &sensor;
}

void MyAlgorithm::setBatteryMeter(const BatteryMeter& meter) {
    this->batteryMeter = &meter;
}

void MyAlgorithm::initializeColorMatrix(int rows, int cols) {
    colorMatrix.resize(rows, std::vector<Color>(cols, WHITE));
    isInitialized = true;
}

Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        // Assume the initial position is the docking station
        int rows = 10; // Get actual rows from the house layout
        int cols = 10; // Get actual cols from the house layout
        initializeColorMatrix(rows, cols);
        colorMatrix[currentRow][currentCol] = GREY;
    }
    return moveToNextCell();
}

Step MyAlgorithm::moveToNextCell() {
    if (historyStack.empty()) {
        historyStack.push({currentRow, currentCol});
    }

    auto current = historyStack.top();
    int row = current.first;
    int col = current.second;

    // Mark current cell as GREY (being processed)
    colorMatrix[row][col] = GREY;

    static const std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    };
    static const std::vector<Step> steps = {
            Step::East, Step::South, Step::West, Step::North
    };

    for (size_t i = 0; i < directions.size(); ++i) {
        int newRow = row + directions[i].first;
        int newCol = col + directions[i].second;
        if (isValidMove(newRow, newCol) && colorMatrix[newRow][newCol] == WHITE) {
            currentRow = newRow;
            currentCol = newCol;
            historyStack.push({newRow, newCol});
            return steps[i];
        }
    }

    // All neighbors are either GREY or BLACK
    if (allNeighborsBlack(row, col)) {
        colorMatrix[row][col] = BLACK;
    }

    // Backtrack
    historyStack.pop();
    if (!historyStack.empty()) {
        auto prev = historyStack.top();
        int prevRow = prev.first;
        int prevCol = prev.second;
        currentRow = prevRow;
        currentCol = prevCol;

        if (row == prevRow && col == prevCol + 1) return Step::West;
        if (row == prevRow && col == prevCol - 1) return Step::East;
        if (row == prevRow + 1 && col == prevCol) return Step::North;
        if (row == prevRow - 1 && col == prevCol) return Step::South;
    }

    return Step::Stay;
}

bool MyAlgorithm::isValidMove(int row, int col) {
    // Assumption: the wallsSensor is updated to the current position
    if (row < 0 || col < 0 || row >= colorMatrix.size() || col >= colorMatrix[0].size()) {
        return false;
    }
    return !wallsSensor->isWall(static_cast<Direction>((row - currentRow) * 2 + (col - currentCol)));
}

bool MyAlgorithm::allNeighborsBlack(int row, int col) {
    static const std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    };

    for (const auto& dir : directions) {
        int newRow = row + dir.first;
        int newCol = col + dir.second;
        if (isValidMove(newRow, newCol) && colorMatrix[newRow][newCol] != BLACK) {
            return false;
        }
    }
    return true;
}
