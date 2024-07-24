#include "ConcreteSensors.h"

ConcreteWallsSensor::ConcreteWallsSensor(const House& house, int row, int col)
        : house(house), row(row), col(col) {}

bool ConcreteWallsSensor::isWall(Direction d) const {
    switch (d) {
        case Direction::North: return house.getCell(row - 1, col) == 'W';
        case Direction::East: return house.getCell(row, col + 1) == 'W';
        case Direction::South: return house.getCell(row + 1, col) == 'W';
        case Direction::West: return house.getCell(row, col - 1) == 'W';
        default: return true;
    }
}

void ConcreteWallsSensor::updatePosition(int row, int col) {
    this->row = row;
    this->col = col;
}

ConcreteDirtSensor::ConcreteDirtSensor(const House& house, int row, int col)
        : house(house), row(row), col(col) {}

int ConcreteDirtSensor::dirtLevel() const {
    char cell = house.getCell(row, col);
    return (cell >= '1' && cell <= '9') ? cell - '0' : 0;
}

void ConcreteDirtSensor::updatePosition(int row, int col) {
    this->row = row;
    this->col = col;
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

void ConcreteBatteryMeter::chargeBattery(int stepsStayedAtDock) {
    int chargeRate = maxBattery / 20;
    currentBattery += stepsStayedAtDock * chargeRate;
    if (currentBattery > maxBattery) {
        currentBattery = maxBattery;
    }
}
