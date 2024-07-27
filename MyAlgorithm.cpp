#include "MyAlgorithm.h"
#include <iostream>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>

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

/*void MyAlgorithm::initialize(int dockrow, int dockcol) {
    if (isInitialized) return;
    isInitialized = true;

    currentRow = dockrow;
    currentCol = dockcol;
    historyStack.push({currentRow, currentCol});
    updateUnexplored(currentRow, currentCol);
    visited[{dockrow, dockcol}] = 20;
}*/
void MyAlgorithm::initialize(int dockrow, int dockcol) {
    if (isInitialized) return;
    isInitialized = true;

    currentRow = dockrow;
    currentCol = dockcol;
    dockingRow = dockrow;
    dockingCol = dockcol;
    historyStack.push({currentRow, currentCol});
    updateUnexplored(currentRow, currentCol);
    visited[{currentRow, currentCol}] = 0;
}

/*
Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        initialize(dockingRow, dockingCol);
    }

    int dirtLevel = dirtSensor->dirtLevel();
    std::pair<int, int> currentPos = {currentRow, currentCol};

    if (dirtLevel > 0) {
        std::cout << "Staying to clean at (" << currentRow << ", " << currentCol << "). Dirt level: " << dirtLevel << std::endl;
        return Step::Stay;
    }

    if (visited.find(currentPos) == visited.end() || visited[currentPos] > 0) {
        visited[currentPos] = 0;  // Mark as fully cleaned
        updateUnexplored(currentRow, currentCol);
    }

    return moveToNextCell();
}
*/
Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        initialize(dockingRow, dockingCol);
    }

    int dirtLevel = dirtSensor->dirtLevel();
    std::pair<int, int> currentPos = {currentRow, currentCol};

    if (dirtLevel > 0) {
        std::cout << "Staying to clean at (" << currentRow << ", " << currentCol << "). Dirt level: " << dirtLevel << std::endl;
        return Step::Stay;
    }

    if (visited.find(currentPos) == visited.end()) {
        visited[currentPos] = 0;
        updateUnexplored(currentRow, currentCol);
    }

    return moveToNextCell();
}

/*Step MyAlgorithm::moveToNextCell() {
    std::pair<int, int> currentPos = {currentRow, currentCol};

    // If there are unexplored directions from the current cell, explore them
    if (!unexplored[currentPos].empty()) {
        Direction dir = unexplored[currentPos].back();
        unexplored[currentPos].pop_back();

        int newRow = currentRow, newCol = currentCol;
        Step step;

        switch (dir) {
            case Direction::North: newRow--; step = Step::North; break;
            case Direction::East:  newCol++; step = Step::East;  break;
            case Direction::South: newRow++; step = Step::South; break;
            case Direction::West:  newCol--; step = Step::West;  break;
        }

        if (isValidMove(newRow, newCol)) {
            currentRow = newRow;
            currentCol = newCol;
            historyStack.push({currentRow, currentCol});
            std::cout << "Moving to (" << currentRow << ", " << currentCol << ")" << std::endl;
            return step;
        }
    }

    // If all directions from current cell are explored, backtrack
    if (!historyStack.empty()) {
        std::pair<int, int> prevPos = historyStack.top();
        historyStack.pop();

        Step backtrackStep;
        if (prevPos.first < currentRow) backtrackStep = Step::North;
        else if (prevPos.first > currentRow) backtrackStep = Step::South;
        else if (prevPos.second < currentCol) backtrackStep = Step::West;
        else backtrackStep = Step::East;

        currentRow = prevPos.first;
        currentCol = prevPos.second;
        std::cout << "Backtracking to (" << currentRow << ", " << currentCol << ")" << std::endl;
        return backtrackStep;
    }

    // If history stack is empty, we've explored everything
    std::cout << "Exploration complete. Returning to docking station." << std::endl;
    return Step::Finish;
}*/
Step MyAlgorithm::moveToNextCell() {
    std::pair<int, int> currentPos = {currentRow, currentCol};

    if (!unexplored[currentPos].empty()) {
        Direction dir = unexplored[currentPos].back();
        unexplored[currentPos].pop_back();

        int newRow = currentRow, newCol = currentCol;
        Step step;

        switch (dir) {
            case Direction::North: newRow--; step = Step::North; break;
            case Direction::East:  newCol++; step = Step::East;  break;
            case Direction::South: newRow++; step = Step::South; break;
            case Direction::West:  newCol--; step = Step::West;  break;
        }

        if (isValidMove(newRow, newCol)) {
            currentRow = newRow;
            currentCol = newCol;
            historyStack.push({currentRow, currentCol});
            std::cout << "Moving to (" << currentRow << ", " << currentCol << ")" << std::endl;
            return step;
        }
    }

    if (!historyStack.empty()) {
        std::pair<int, int> prevPos = historyStack.top();
        historyStack.pop();

        Step backtrackStep;
        if (prevPos.first < currentRow) backtrackStep = Step::North;
        else if (prevPos.first > currentRow) backtrackStep = Step::South;
        else if (prevPos.second < currentCol) backtrackStep = Step::West;
        else backtrackStep = Step::East;

        currentRow = prevPos.first;
        currentCol = prevPos.second;
        std::cout << "Backtracking to (" << currentRow << ", " << currentCol << ")" << std::endl;
        return backtrackStep;
    }

    std::cout << "Exploration complete. Returning to docking station." << std::endl;
    return Step::Finish;
}
Step MyAlgorithm::backtrack() {
    if (historyStack.empty()) {
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
void MyAlgorithm::resetExploration() {
    visited.clear();
    unexplored.clear();
    historyStack = std::stack<std::pair<int, int>>();
    initialize(dockingRow, dockingCol);
}

std::vector<Step> MyAlgorithm::findPathToDock() {
    std::queue<std::pair<int, int>> queue;
    std::unordered_map<int, std::unordered_map<int, std::pair<int, int>>> cameFrom;
    std::unordered_map<int, std::unordered_map<int, bool>> exploredCells;

    queue.push({currentRow, currentCol});
    exploredCells[currentRow][currentCol] = true;

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current.first == dockingRow && current.second == dockingCol) {
            // Found the path, reconstruct it
            return reconstructPath(cameFrom, current);
        }

        // Check all four directions
        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& dir : directions) {
            int newRow = current.first + dir.first;
            int newCol = current.second + dir.second;

            if (!exploredCells[newRow][newCol] && !wallsSensor->isWall(directionFromOffset(dir))) {
                queue.push({newRow, newCol});
                exploredCells[newRow][newCol] = true;
                cameFrom[newRow][newCol] = current;
            }
        }
    }

    // If we get here, there's no path to the docking station
    return {};
}
std::vector<Step> MyAlgorithm::reconstructPath(
        const std::unordered_map<int, std::unordered_map<int, std::pair<int, int>>>& cameFrom,
        std::pair<int, int> current) {
    std::vector<Step> path;
    while (current.first != currentRow || current.second != currentCol) {
        auto prev = cameFrom.at(current.first).at(current.second);
        if (prev.first < current.first) path.push_back(Step::South);
        else if (prev.first > current.first) path.push_back(Step::North);
        else if (prev.second < current.second) path.push_back(Step::East);
        else path.push_back(Step::West);
        current = prev;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
Direction MyAlgorithm::directionFromOffset(const std::pair<int, int>& offset) {
    if (offset.first == -1) return Direction::North;
    if (offset.first == 1) return Direction::South;
    if (offset.second == -1) return Direction::West;
    return Direction::East;
}

