#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <vector>

class House {
public:
    House(const std::string& filePath = ""); // Default parameter for constructor
    bool isValid() const;
    int getMaxSteps() const;
    int getMaxBattery() const;
    int getRows() const;
    int getCols() const;
    int getDockingStationRow() const;
    int getDockingStationCol() const;
    char getCell(int row, int col) const;
    void cleanCell(int row, int col);

private:
    std::string name;
    int maxSteps;
    int maxBattery;
    int rows;
    int cols;
    std::vector<std::vector<char>> layout;
    bool valid;
    int dockingStationRow;
    int dockingStationCol;

    void parseHouseFile(const std::string& filePath);
};

#endif // HOUSE_H
