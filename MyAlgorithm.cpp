#include "MyAlgorithm.h"
#include <iostream>
#include <queue>
#include <cmath>
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

void MyAlgorithm::updatePosition(int row, int col) {
    currentRow = row;
    currentCol = col;
}

void MyAlgorithm::initialize(int dockrow, int dockcol) {
    if (isInitialized) return;
    isInitialized = true;
    currentRow = dockrow;
    currentCol = dockcol;
    updateUnexplored(currentRow, currentCol);
    visited[{currentRow, currentCol}] = 0;
}
/*
Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        initialize(dockingRow, dockingCol);
    }

    int currentBattery = batteryMeter->getBatteryState();
    std::vector<Step> pathToDock = findPathToDock();
    int distanceToDock = pathToDock.size();

    if (currentBattery <= distanceToDock + 2) {
        if (pathToDock.empty()) {
            std::cout << "No path to docking station found!" << std::endl;
            return Step::Stay;
        }
        return pathToDock[0];
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
*/
Step MyAlgorithm::nextStep() {
    if (!isInitialized) {
        initialize(dockingRow, dockingCol);
    }

    std::cout << "Current position: (" << currentRow << ", " << currentCol << ")" << std::endl;
    std::cout << "Visited cells: " << visited.size() << std::endl;
    std::cout << "Unexplored directions: " << unexplored[{currentRow, currentCol}].size() << std::endl;

    int currentBattery = batteryMeter->getBatteryState();
    std::vector<Step> pathToDock = findPathToDock();
    int distanceToDock = pathToDock.size();

    if (currentBattery <= distanceToDock + 2) {
        if (pathToDock.empty()) {
            std::cout << "No path to docking station found!" << std::endl;
            return Step::Stay;
        }
        return pathToDock[0];
    }

    int dirtLevel = dirtSensor->dirtLevel();
    std::pair<int, int> currentPos = {currentRow, currentCol};

    if (dirtLevel > 0) {
        std::cout << "Staying to clean at (" << currentRow << ", " << currentCol << "). Dirt level: " << dirtLevel << std::endl;
        cleanedCells.insert(currentPos);
        return Step::Stay;
    }

    if (visited.find(currentPos) == visited.end()) {
        visited[currentPos] = 0;
        updateUnexplored(currentRow, currentCol);
    }

    return moveToNextCell();
}
/*
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

        if (isValidMove(newRow, newCol) && visited.find({newRow, newCol}) == visited.end()) {
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
*/
Step MyAlgorithm::moveToNextCell() {
    std::pair<int, int> currentPos = {currentRow, currentCol};

    std::cout << "Moving from (" << currentRow << ", " << currentCol << ")" << std::endl;

    if (!unexplored[currentPos].empty()) {
        for (auto it = unexplored[currentPos].rbegin(); it != unexplored[currentPos].rend(); ++it) {
            Direction dir = *it;
            int newRow = currentRow, newCol = currentCol;
            Step step;

            switch (dir) {
                case Direction::North: newRow--; step = Step::North; break;
                case Direction::East:  newCol++; step = Step::East;  break;
                case Direction::South: newRow++; step = Step::South; break;
                case Direction::West:  newCol--; step = Step::West;  break;
            }

            if (isValidMove(newRow, newCol) &&
                (visited.find({newRow, newCol}) == visited.end() ||
                 cleanedCells.find({newRow, newCol}) == cleanedCells.end())) {
                unexplored[currentPos].pop_back();
                currentRow = newRow;
                currentCol = newCol;
                historyStack.push({currentRow, currentCol});
                std::cout << "Moving to (" << currentRow << ", " << currentCol << ")" << std::endl;
                return step;
            }
        }
        unexplored[currentPos].clear();
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
/*
bool MyAlgorithm::isValidMove(int row, int col) {
    if (row < 0 || col < 0) return false;
    Direction dir;
    if (row < currentRow) dir = Direction::North;
    else if (row > currentRow) dir = Direction::South;
    else if (col < currentCol) dir = Direction::West;
    else if (col > currentCol) dir = Direction::East;
    else return false;
    return !wallsSensor->isWall(dir);
}
 */
/*
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
*/
bool MyAlgorithm::isValidMove(int row, int col) {
    Direction dir;
    if (row < currentRow) dir = Direction::North;
    else if (row > currentRow) dir = Direction::South;
    else if (col < currentCol) dir = Direction::West;
    else if (col > currentCol) dir = Direction::East;
    else return false;
    return !wallsSensor->isWall(dir);
}
void MyAlgorithm::updateUnexplored(int row, int col) {
    std::vector<Direction> directions = {Direction::North, Direction::East, Direction::South, Direction::West};
    unexplored[{row, col}].clear();  // Clear existing unexplored directions
    for (Direction dir : directions) {
        int newRow = row, newCol = col;
        switch (dir) {
            case Direction::North: newRow--; break;
            case Direction::East: newCol++; break;
            case Direction::South: newRow++; break;
            case Direction::West: newCol--; break;
        }
        if (isValidMove(newRow, newCol)&&visited.find({newRow, newCol}) == visited.end()) {
            unexplored[{row, col}].push_back(dir);
        }
    }
}
std::vector<Step> MyAlgorithm::findPathToDock() {
    std::queue<std::pair<int, int>> queue;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
    std::map<std::pair<int, int>, bool> exploredCells;

    queue.push({currentRow, currentCol});
    exploredCells[{currentRow, currentCol}] = true;

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current.first == dockingRow && current.second == dockingCol) {
            return reconstructPath(cameFrom, current);
        }

        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& dir : directions) {
            int newRow = current.first + dir.first;
            int newCol = current.second + dir.second;

            if (exploredCells.find({newRow, newCol}) == exploredCells.end() &&
                !wallsSensor->isWall(directionFromOffset(dir))) {
                queue.push({newRow, newCol});
                exploredCells[{newRow, newCol}] = true;
                cameFrom[{newRow, newCol}] = current;
            }
        }
    }

    return {};
}

Direction MyAlgorithm::directionFromOffset(const std::pair<int, int>& offset) {
    if (offset.first == -1) return Direction::North;
    if (offset.first == 1) return Direction::South;
    if (offset.second == -1) return Direction::West;
    return Direction::East;
}

std::vector<Step> MyAlgorithm::reconstructPath(
        const std::map<std::pair<int, int>, std::pair<int, int>>& cameFrom,
        std::pair<int, int> current) {
    std::vector<Step> path;
    while (current.first != currentRow || current.second != currentCol) {
        auto it = cameFrom.find(current);
        if (it == cameFrom.end()) {
            // This should never happen if the path exists
            std::cerr << "Error in reconstructing path" << std::endl;
            return {};
        }
        auto prev = it->second;
        if (prev.first < current.first) path.push_back(Step::South);
        else if (prev.first > current.first) path.push_back(Step::North);
        else if (prev.second < current.second) path.push_back(Step::East);
        else path.push_back(Step::West);
        current = prev;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
void MyAlgorithm::resetExploration() {
    visited.clear();
    unexplored.clear();
    historyStack = std::stack<std::pair<int, int>>();
    isInitialized = false;
    initialize(dockingRow, dockingCol);
}

void MyAlgorithm::setDockingStation(int row, int col) {
    dockingRow = row;
    dockingCol = col;
}