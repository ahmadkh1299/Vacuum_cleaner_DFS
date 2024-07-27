#include "House.h"
#include <iostream>
#include <algorithm>

House::House()
        : rows(0), cols(0), dockingStationRow(-1), dockingStationCol(-1), total_dirt(0) {}

House::House(const std::vector<std::string>& layout_v)
        : dockingStationRow(-1), dockingStationCol(-1), total_dirt(0) {
    // Print the received layout
    std::cout << "Received Layout:\n";
    for (const auto& row : layout_v) {
        std::cout << row << '\n';
    }

    // Initialize the matrix with the layout
    initializeMatrix(layout_v);
    findDockingStation();
    updateDirtCount();
    printMatrix();
}

void House::addWallsPadding(std::vector<std::string>& layout_v) {
    // Ensure each row has the correct number of columns, padding with spaces
    for (auto& row : layout_v) {
        if (row.size() < cols) {
            row.append(cols - row.size(), '0');
        }
    }

    // Add walls only if not already present
    if (layout_v.size() < rows) {
        // Add missing rows filled with '0'
        while (layout_v.size() < rows) {
            layout_v.push_back(std::string(cols, '0'));
        }
    }

    // Enclose the layout with walls
    for (auto& row : layout_v) {
        if (row[0] != 'W') row.insert(row.begin(), 'W');
        if (row.back() != 'W') row.push_back('W');
    }

    // If necessary, add a row of walls at the top and bottom
    if (layout_v.front()[0] != 'W') {
        layout_v.insert(layout_v.begin(), std::string(cols + 2, 'W'));
    }
    if (layout_v.back()[0] != 'W') {
        layout_v.push_back(std::string(cols + 2, 'W'));
    }
}

void House::initializeMatrix(const std::vector<std::string>& layout_v) {
    rows = static_cast<int>(layout_v.size());
    cols = static_cast<int>(layout_v[0].size());
    house_matrix.resize(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            char cell = layout_v[i][j];
            if (cell == 'W') {
                house_matrix[i][j] = -1;
            } else if (cell >= '1' && cell <= '9') {
                house_matrix[i][j] = cell - '0';
                total_dirt += house_matrix[i][j];
            } else if (cell == 'D') {
                dockingStationRow = i;
                dockingStationCol = j;
                house_matrix[i][j] = 20;
            } else {
                house_matrix[i][j] = 0;
            }
        }
    }
}

void House::findDockingStation() {
    if (dockingStationRow == -1 || dockingStationCol == -1) {
        throw std::runtime_error("Docking station 'D' not found in layout");
    }
}

void House::updateDirtCount() {
    total_dirt = 0;
    for (const auto& row : house_matrix) {
        for (int cell : row) {
            if (cell > 0 && cell < 20) {
                total_dirt += cell;
            }
        }
    }
}

void House::printMatrix() const {
    std::cout << "House matrix:\n";
    std::cout << "The number is: " << dockingStationRow << std::endl;
    std::cout << "The number is: " << dockingStationCol << std::endl;

    for (const auto& row : house_matrix) {
        for (int cell : row) {
            if (cell == -1) {
                std::cout << "W ";
            } else if (cell == 20) {
                std::cout << "D ";
            } else {
                std::cout << cell << ' ';
            }
        }
        std::cout << '\n';
    }
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

int House::getCell(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return -1; // Boundary walls represented by -1
    }
    return house_matrix[row][col];
}

void House::cleanCell(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return; // Ignore out-of-bound cells
    }

    if (house_matrix[row][col] > 0 && house_matrix[row][col] < 20) {
        house_matrix[row][col]--;
        total_dirt--;
    }
}

bool House::isHouseClean() const {
    return total_dirt < 1;
}
