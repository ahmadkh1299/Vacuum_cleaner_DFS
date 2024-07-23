#include "MyAlgorithm.h"
#include <iostream>

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

void MyAlgorithm::initialize() {
    isInitialized = true;
    currentRow = 0;
    currentCol = 0;
    historyStack.push({currentRow, currentCol});
    updateUnexplored(currentRow, currentCol);
}

Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        initialize();
    }

    // Clean the current cell if it has dirt
    int dirtLevel = dirtSensor->dirtLevel();
    if (dirtLevel > 0) {
        visited[{currentRow, currentCol}] = dirtLevel - 1;
        return Step::Stay;
    }

    // Move to the next unexplored cell or backtrack if necessary
    return moveToNextCell();
}

Step MyAlgorithm::moveToNextCell() {
    // If the current cell is fully explored, backtrack
    if (unexplored[{currentRow, currentCol}].empty()) {
        return backtrack();
    }

    // Move to the next unexplored direction
    Direction dir = unexplored[{currentRow, currentCol}].back();
    unexplored[{currentRow, currentCol}].pop_back();

    switch (dir) {
        case Direction::North:
            if (isValidMove(currentRow - 1, currentCol)) {
                currentRow--;
                historyStack.push({currentRow, currentCol});
                updateUnexplored(currentRow, currentCol);
                return Step::North;
            }
            break;
        case Direction::East:
            if (isValidMove(currentRow, currentCol + 1)) {
                currentCol++;
                historyStack.push({currentRow, currentCol});
                updateUnexplored(currentRow, currentCol);
                return Step::East;
            }
            break;
        case Direction::South:
            if (isValidMove(currentRow + 1, currentCol)) {
                currentRow++;
                historyStack.push({currentRow, currentCol});
                updateUnexplored(currentRow, currentCol);
                return Step::South;
            }
            break;
        case Direction::West:
            if (isValidMove(currentRow, currentCol - 1)) {
                currentCol--;
                historyStack.push({currentRow, currentCol});
                updateUnexplored(currentRow, currentCol);
                return Step::West;
            }
            break;
    }

    // If the move is not valid, try the next direction
    return moveToNextCell();
}

Step MyAlgorithm::backtrack() {
    if (historyStack.empty()) {
        return Step::Finish;
    }

    std::pair<int, int> prev = historyStack.top();
    historyStack.pop();

    int prevRow = prev.first;
    int prevCol = prev.second;

    if (currentRow == prevRow && currentCol == prevCol + 1) {
        currentCol--;
        return Step::West;
    } else if (currentRow == prevRow && currentCol == prevCol - 1) {
        currentCol++;
        return Step::East;
    } else if (currentRow == prevRow + 1 && currentCol == prevCol) {
        currentRow--;
        return Step::North;
    } else if (currentRow == prevRow - 1 && currentCol == prevCol) {
        currentRow++;
        return Step::South;
    }

    return Step::Stay;
}

bool MyAlgorithm::isValidMove(int row, int col) {
    if (row == currentRow - 1 && wallsSensor->isWall(Direction::North)) return false;
    if (row == currentRow + 1 && wallsSensor->isWall(Direction::South)) return false;
    if (col == currentCol - 1 && wallsSensor->isWall(Direction::West)) return false;
    if (col == currentCol + 1 && wallsSensor->isWall(Direction::East)) return false;
    return true;
}

void MyAlgorithm::updateUnexplored(int row, int col) {
    std::vector<Direction> directions = {Direction::North, Direction::East, Direction::South, Direction::West};
    for (Direction dir : directions) {
        int newRow = row, newCol = col;
        switch (dir) {
            case Direction::North: newRow--; break;
            case Direction::East: newCol++; break;
            case Direction::South: newRow++; break;
            case Direction::West: newCol--; break;
        }
        if (isValidMove(newRow, newCol) && visited.find({newRow, newCol}) == visited.end()) {
            unexplored[{row, col}].push_back(dir);
        }
    }
}
