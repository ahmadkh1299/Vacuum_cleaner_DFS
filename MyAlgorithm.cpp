#include "MyAlgorithm.h"
#include <iostream>
#include <map>

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

void MyAlgorithm::setDockingStation(int Row, int Col) {
    dockingRow = Row;
    dockingCol = Col;
}

void MyAlgorithm::initialize(int dockrow, int dockcol) {
    if (isInitialized) return;
    isInitialized = true;

    currentRow = dockrow;
    currentCol = dockcol;
    historyStack.push({currentRow, currentCol});
    updateUnexplored(currentRow, currentCol);
    visited[{dockrow, dockcol}] = 20;
}

Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        initialize(dockingRow, dockingCol);
    }

    int remainingBattery = batteryMeter->getBatteryState();
    if (remainingBattery <= historyStack.size()+2) {
        return backtrackToDocking();
    }

    int dirtLevel = dirtSensor->dirtLevel();
    if (dirtLevel > 0 && dirtLevel <= 9) {
        visited[{currentRow, currentCol}] = dirtLevel - 1;
        return Step::Stay;
    }

    if (visited.find({currentRow, currentCol}) == visited.end()) {
        updateUnexplored(currentRow, currentCol);
    }

    return moveToNextCell();
}

Step MyAlgorithm::moveToNextCell() {
    if (unexplored[{currentRow, currentCol}].empty()) {
        return backtrack();// we need a function to start again in the cells not visited yet
    }

    Direction dir = unexplored[{currentRow, currentCol}].back();
    unexplored[{currentRow, currentCol}].pop_back();

    switch (dir) {
        case Direction::North:
            if (isValidMove(currentRow - 1, currentCol)) {
                currentRow--;
                historyStack.push({currentRow, currentCol});
                std::cout << "Moving North to (" << currentRow << ", " << currentCol << ")\n";
                return Step::North;
            }
            break;
        case Direction::East:
            if (isValidMove(currentRow, currentCol + 1)) {
                currentCol++;
                historyStack.push({currentRow, currentCol});
                std::cout << "Moving East to (" << currentRow << ", " << currentCol << ")\n";
                return Step::East;
            }
            break;
        case Direction::South:
            if (isValidMove(currentRow + 1, currentCol)) {
                currentRow++;
                historyStack.push({currentRow, currentCol});
                std::cout << "Moving South to (" << currentRow << ", " << currentCol << ")\n";
                return Step::South;
            }
            break;
        case Direction::West:
            if (isValidMove(currentRow, currentCol - 1)) {
                currentCol--;
                historyStack.push({currentRow, currentCol});
                std::cout << "Moving West to (" << currentRow << ", " << currentCol << ")\n";
                return Step::West;
            }
            break;
    }
    return moveToNextCell();
}

Step MyAlgorithm::backtrack() {
    if (historyStack.empty()) {
        if (dockingCol==currentCol && dockingRow==currentRow)
        { startcharging= true; }
        return moveToNextCell();// we need to charge , update battery , and start again
    }

    std::pair<int, int> prev = historyStack.top();
    historyStack.pop();

    int prevRow = prev.first;
    int prevCol = prev.second;

    if (currentRow == prevRow && currentCol == prevCol + 1) {
        currentCol--;
        std::cout << "Backtracking West to (" << currentRow << ", " << currentCol << ")\n";
        return Step::West;
    } else if (currentRow == prevRow && currentCol == prevCol - 1) {
        currentCol++;
        std::cout << "Backtracking East to (" << currentRow << ", " << currentCol << ")\n";
        return Step::East;
    } else if (currentRow == prevRow + 1 && currentCol == prevCol) {
        currentRow--;
        std::cout << "Backtracking North to (" << currentRow << ", " << currentCol << ")\n";
        return Step::North;
    } else if (currentRow == prevRow - 1 && currentCol == prevCol) {
        currentRow++;
        std::cout << "Backtracking South to (" << currentRow << ", " << currentCol << ")\n";
        return Step::South;
    }

    return Step::Stay;
}

Step MyAlgorithm::backtrackToDocking() {
    if (historyStack.empty()) {
        return moveToNextCell();
    }

    std::pair<int, int> prev = historyStack.top();
    historyStack.pop();

    int prevRow = prev.first;
    int prevCol = prev.second;

    if (currentRow == prevRow && currentCol == prevCol + 1) {
        currentCol--;
        std::cout << "Backtracking to Docking West to (" << currentRow << ", " << currentCol << ")\n";
        return Step::West;
    } else if (currentRow == prevRow && currentCol == prevCol - 1) {
        currentCol++;
        std::cout << "Backtracking to Docking East to (" << currentRow << ", " << currentCol << ")\n";
        return Step::East;
    } else if (currentRow == prevRow + 1 && currentCol == prevCol) {
        currentRow--;
        std::cout << "Backtracking to Docking North to (" << currentRow << ", " << currentCol << ")\n";
        return Step::North;
    } else if (currentRow == prevRow - 1 && currentCol == prevCol) {
        currentRow++;
        std::cout << "Backtracking to Docking South to (" << currentRow << ", " << currentCol << ")\n";
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
std::stack<Step> MyAlgorithm::findPathToDocking(const std::stack<Step>& history) {
    std::stack<Step> path;
    std::stack<Step> temp_history = history;  // Copy of the original history stack to preserve order
    std::stack<Step> reversed = std::stack<Step>();
    while (!temp_history.empty()) {
        reversed.push(temp_history.top());
        temp_history.pop();
    }
    temp_history = reversed;
    reversed = std::stack<Step>();
    while (!temp_history.empty()) {
        Step move = reverseDirection(temp_history.top());
        temp_history.pop();
        if (move != Step::Stay) {
            reversed.push(move);
        }
    }
    return reversed;
}
Step MyAlgorithm::reverseDirection(Step direction) {
    switch (direction) {
        case Step::North:
            return Step::South;
        case Step::East:
            return Step::West;
        case Step::South:
            return Step::North;
        case Step::West:
            return Step::East;
        case Step::Stay:
        default:
            return Step::Stay;
    }
}