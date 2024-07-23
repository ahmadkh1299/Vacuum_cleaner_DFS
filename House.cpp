#include "House.h"
#include <fstream>
#include <iostream>
#include <sstream>

House::House(const std::string& filePath) : valid(true), dockingStationRow(-1), dockingStationCol(-1) {
    parseHouseFile(filePath);
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
        std::istringstream iss(line);
        if (lineNumber == 1) {
            name = line;
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
                for (int i = 0; i < std::min((int)line.size(), cols); ++i) {
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

    if (layout.size() != rows || dockingStationRow == -1 || dockingStationCol == -1) {
        std::cerr << "Invalid house file: incorrect number of rows or missing docking station." << std::endl;
        valid = false;
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
    if (layout[row][col] >= '1' && layout[row][col] <= '9') {
        layout[row][col]--;
    }
}
