#include "MyAlgorithm.h"
#include <iostream>
#include <cmath>

MyAlgorithm::MyAlgorithm()
        : maxSteps(0), wallsSensor(nullptr), dirtSensor(nullptr), batteryMeter(nullptr),
          currentRow(0), currentCol(0), dockingRow(0), dockingCol(0), isInitialized(false) {}

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
    if (isInitialized) return;
    isInitialized = true;

    // Assume that the initial position is the docking station
    currentRow = dockingRow;
    currentCol = dockingCol;
    historyStack.push({currentRow, currentCol});
    updateUnexplored(currentRow, currentCol);
    pathToDock.clear();
}

Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        initialize();
    }

    // Check if we need to return to the docking station
    if (needToReturnToDock()) {
        return returnToDock();
    }

    // Clean the current cell if it has dirt
    int dirtLevel = dirtSensor->dirtLevel();
    if (dirtLevel > 0) {
        visited[{currentRow, currentCol}] = dirtLevel - 1;
        return Step::Stay;
    }

    // If the house is clean and we're at the docking station, finish
    if (isHouseClean() && pathToDock.empty()) {
        return Step::Finish;
    }

    // Continue exploring or backtrack if all nearby areas are explored
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

    Step step = Step::Stay;
    switch (dir) {
        case Direction::North:
            if (isValidMove(currentRow - 1, currentCol)) {
                currentRow--;
                step = Step::North;
            }
            break;
        case Direction::East:
            if (isValidMove(currentRow, currentCol + 1)) {
                currentCol++;
                step = Step::East;
            }
            break;
        case Direction::South:
            if (isValidMove(currentRow + 1, currentCol)) {
                currentRow++;
                step = Step::South;
            }
            break;
        case Direction::West:
            if (isValidMove(currentRow, currentCol - 1)) {
                currentCol--;
                step = Step::West;
            }
            break;
    }

    if (step != Step::Stay) {
        historyStack.push({currentRow, currentCol});
        updateUnexplored(currentRow, currentCol);
        pathToDock.push_back(step);
    }

    return step != Step::Stay ? step : moveToNextCell();
}

Step MyAlgorithm::backtrack() {
    // If all cells have been visited, the task is complete
    if (historyStack.empty()) {
        return Step::Finish;
    }

    std::pair<int, int> prev = historyStack.top();
    historyStack.pop();

    int prevRow = prev.first;
    int prevCol = prev.second;

    Step step = Step::Stay;
    if (currentRow == prevRow && currentCol == prevCol + 1) {
        currentCol--;
        step = Step::West;
    } else if (currentRow == prevRow && currentCol == prevCol - 1) {
        currentCol++;
        step = Step::East;
    } else if (currentRow == prevRow + 1 && currentCol == prevCol) {
        currentRow--;
        step = Step::North;
    } else if (currentRow == prevRow - 1 && currentCol == prevCol) {
        currentRow++;
        step = Step::South;
    }

    if (step != Step::Stay) {
        pathToDock.pop_back();
    }

    return step;
}

bool MyAlgorithm::isValidMove(int row, int col) {
    if (row == currentRow - 1 && wallsSensor->isWall(Direction::North)) return false;
    if (row == currentRow + 1 && wallsSensor->isWall(Direction::South)) return false;
    if (col == currentCol - 1 && wallsSensor->isWall(Direction::West)) return false;
    if (col == currentCol + 1 && wallsSensor->isWall(Direction::East)) return false;
    return true;
}

void MyAlgorithm::updateUnexplored(int row, int col) {
    unexplored[{row, col}].clear(); // Re-evaluate all directions
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

bool MyAlgorithm::needToReturnToDock() {
    int batteryLeft = batteryMeter->getBatteryState();
    int stepsToDoc = pathToDock.size();
    return batteryLeft <= stepsToDoc + 5; // +5 for safety margin
}



bool MyAlgorithm::isHouseClean() const {
    for (const auto& pair : visited) {
        if (pair.second > 0) return false;
    }
    return true;
}

bool MyAlgorithm::atDockingStation() const {
    return currentRow == dockingRow && currentCol == dockingCol;
}


Step MyAlgorithm::returnToDock() {
    if (pathToDock.empty()) {
        return Step::Stay; // We're at the docking station
    }

    Step reverseStep = pathToDock.back();
    pathToDock.pop_back();

    switch (reverseStep) {
        case Step::North: 
            currentRow++;
            return Step::South;
        case Step::South:
            currentRow--;
            return Step::North;
        case Step::East:
            currentCol--;
            return Step::West;
        case Step::West:
            currentCol++;
            return Step::East;
        default:
            return Step::Stay;
    }
}

bool MyAlgorithm::isHouseClean() {
    for (const auto& pair : visited) {
        if (pair.second > 0) return false;
    }
    return true;
}

bool MyAlgorithm::atDockingStation() {
    return currentRow == dockingRow && currentCol == dockingCol;
}