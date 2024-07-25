#include "House.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

House::House(const std::string& filePath) : valid(true), dockingStationRow(-1), dockingStationCol(-1), curr_dirt(0) {
    if (!filePath.empty()) {
        parseHouseFile(filePath);
    }
}

void House::parseHouseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        valid = false;
        return;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        if (lineNumber == 1) {
            name = line; // Ignore the house name
        } else if (lineNumber == 2) {
            if (sscanf(line.c_str(), "MaxSteps = %d", &maxSteps) != 1) {
                std::cerr << "Invalid MaxSteps line." << std::endl;
                valid = false;
                return;
            }
        } else if (lineNumber == 3) {
            if (sscanf(line.c_str(), "MaxBattery = %d", &maxBattery) != 1) {
                std::cerr << "Invalid MaxBattery line." << std::endl;
                valid = false;
                return;
            }
        } else if (lineNumber == 4) {
            if (sscanf(line.c_str(), "Rows = %d", &rows) != 1) {
                std::cerr << "Invalid Rows line." << std::endl;
                valid = false;
                return;
            }
        } else if (lineNumber == 5) {
            if (sscanf(line.c_str(), "Cols = %d", &cols) != 1) {
                std::cerr << "Invalid Cols line." << std::endl;
                valid = false;
                return;
            }
        } else {
            if (lineNumber - 6 < rows) {
                std::vector<char> row(cols, ' ');
                for (int i = 0; i < std::min(static_cast<int>(line.size()), cols); ++i) {
                    row[i] = line[i];
                    if (row[i] == 'D') {
                        dockingStationRow = lineNumber - 6;
                        dockingStationCol = i;
                    }
                }
                layout.push_back(row);
            }
        }
    }

    // Fill missing rows with empty spaces
    while (layout.size() < rows) {
        layout.push_back(std::vector<char>(cols, ' '));
    }

    // Check if docking station is present
    if (dockingStationRow == -1 || dockingStationCol == -1) {
        std::cerr << "Invalid house file: missing docking station." << std::endl;
        valid = false;
    }

    // Initialize curr_dirt
    updateDirtCount();
}

void House::updateDirtCount() {
    curr_dirt = 0;
    for (const auto& row : layout) {
        for (char cell : row) {
            if (cell >= '1' && cell <= '9') {
                curr_dirt += cell - '0';
            }
        }
    }
}

bool House::isValid() const {
    return valid;
}

int House::getMaxSteps() const {
    return maxSteps;
}

int House::getMaxBattery() const {
    return maxBattery;
}

int House::getRows() const {
    return rows;
}

int House::getCols() const {
    return cols;
}

int House::getDockingStationRow() const {
    return dockingStationRow;
}

int House::getDockingStationCol() const {
    return dockingStationCol;
}

char House::getCell(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return 'W'; // Boundary walls
    }
    return layout[row][col];
}

void House::cleanCell(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return; // Ignore out-of-bound cells
    }

    if (layout[row][col] >= '1' && layout[row][col] <= '9') {
        layout[row][col]--;
        curr_dirt--;
    }
}

bool House::isHouseClean() const {
    return curr_dirt < 1;
}