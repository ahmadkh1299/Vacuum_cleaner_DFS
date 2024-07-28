#include <iostream>
#include "ConcreteSensors.h"

ConcreteWallsSensor::ConcreteWallsSensor(const House& house, int row, int col)
        : house(house), row(row), col(col) {}

bool ConcreteWallsSensor::isWall(Direction d) const {
    switch (d) {
        case Direction::North: return house.getCell(row - 1, col) == -1;
        case Direction::East: return house.getCell(row, col + 1) == -1;
        case Direction::South: return house.getCell(row + 1, col) == -1;
        case Direction::West: return house.getCell(row, col - 1) == -1;
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
    return house.getCell(row,col);
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
        std::cout << "currentBattery (" << currentBattery << "=  " << std::endl;
        currentBattery--;
        std::cout << "currentBattery (" << currentBattery << "=  " << std::endl;

    }
}
void ConcreteBatteryMeter::chargeBattery(int stepsStayedAtDock) {
    std::cout << "currentBattery (" << currentBattery << "=  " << std::endl;
    double chargeRate = maxBattery / 20.0;
    currentBattery += (stepsStayedAtDock * chargeRate);
    if (currentBattery > maxBattery) {
        currentBattery = maxBattery;
    }
    std::cout << "currentBattery (" << currentBattery << "=  " << std::endl;

}

int ConcreteBatteryMeter::getmaxBattery() const{ return maxBattery;}

