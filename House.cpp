#include "House.h"
#include <iostream>
#include <algorithm>

House::House(const std::vector<std::string>& layout_v)
        : dockingStationRow(-1), dockingStationCol(-1), total_dirt(0) {
    std::vector<std::string> layout_copy = layout_v;
    addWallsPadding(layout_copy);
    initializeMatrix(layout_copy);
    findDockingStation();
    updateDirtCount();
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

void House::addWallsPadding(std::vector<std::string>& layout_v) {
    int max_length = 0;

    for (const auto& row : layout_v) {
        if (row.size() > max_length) {
            max_length = row.size();
        }
    }

    for (auto& row : layout_v) {
        while (row.size() < max_length) {
            row.push_back('W');
        }
    }

    bool needs_walls = false;

    // Check top and bottom rows
    for (char c : layout_v[0]) {
        if (c != 'W') {
            needs_walls = true;
            break;
        }
    }
    for (char c : layout_v[layout_v.size() - 1]) {
        if (c != 'W') {
            needs_walls = true;
            break;
        }
    }

    // Check left and right columns
    if (!needs_walls) {
        for (const auto& row : layout_v) {
            if (row[0] != 'W' || row[max_length - 1] != 'W') {
                needs_walls = true;
                break;
            }
        }
    }

    if (needs_walls) {
        for (auto& row : layout_v) {
            row.insert(row.begin(), 'W');
            row.push_back('W');
        }
        std::string wall_row(max_length + 2, 'W');
        layout_v.insert(layout_v.begin(), wall_row);
        layout_v.push_back(wall_row);
    }
}

void House::printLayout() const {
    std::cout << "House Layout:" << std::endl;
    for (const auto& row : house_matrix) {
        for (const auto& cell : row) {
            if (cell == -1) {
                std::cout << 'W';
            } else if (cell == 20) {
                std::cout << 'D';
            } else {
                std::cout << cell;
            }
        }
        std::cout << std::endl;
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
