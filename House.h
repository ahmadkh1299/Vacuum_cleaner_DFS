#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <vector>

class House {
public:
    House();
    House(const std::vector<std::string>& layout_v);

    int getRows() const;
    int getCols() const;
    int getDockingStationRow() const;
    int getDockingStationCol() const;
    int getCell(int row, int col) const;
    void cleanCell(int row, int col);
    bool isHouseClean() const;
    int calculateTotalDirt() const;
    void printMatrix() const;

private:
    int rows;
    int cols;
    int dockingStationRow;
    int dockingStationCol;
    int total_dirt;
    std::vector<std::vector<int>> house_matrix;
    bool valid;

    void addWallsPadding(std::vector<std::string>& layout_v);
    void initializeMatrix(const std::vector<std::string>& layout_v);
    void findDockingStation();
    void updateDirtCount();

};

#endif // HOUSE_H
