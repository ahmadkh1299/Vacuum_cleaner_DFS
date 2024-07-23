#include "ConcreteSensors.h"

ConcreteWallsSensor::ConcreteWallsSensor(const House& house, int& row, int& col)
        : house(house), row(row), col(col) {}

bool ConcreteWallsSensor::isWall(Direction d) const {
    int newRow = row;
    int newCol = col;
    switch (d) {
        case Direction::North: newRow--; break;
        case Direction::East: newCol++; break;
        case Direction::South: newRow++; break;
        case Direction::West: newCol--; break;
    }
    return house.getCell(newRow, newCol) == 'W';
}

ConcreteDirtSensor::ConcreteDirtSensor(const House& house, int& row, int& col)
        : house(house), row(row), col(col) {}

int ConcreteDirtSensor::dirtLevel() const {
    char cell = house.getCell(row, col);
    return (cell >= '0' && cell <= '9') ? cell - '0' : 0;
}

ConcreteBatteryMeter::ConcreteBatteryMeter(int maxBattery)
        : maxBattery(maxBattery), currentBattery(maxBattery) {}

std::size_t ConcreteBatteryMeter::getBatteryState() const {
    return currentBattery;
}

void ConcreteBatteryMeter::useBattery() {
    if (currentBattery > 0) {
        --currentBattery;
    }
}

void ConcreteBatteryMeter::rechargeBattery() {
    currentBattery = maxBattery;
}
