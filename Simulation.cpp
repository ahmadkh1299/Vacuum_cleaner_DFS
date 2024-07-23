#include "Simulation.h"
#include <iostream>

Simulation::Simulation()
        : house(""), currentRow(0), currentCol(0), algorithm(nullptr) {}

void Simulation::readHouseFile(const std::string& houseFilePath) {
    house = House(houseFilePath);
    if (!house.isValid()) {
        std::cerr << "Invalid house file." << std::endl;
        exit(EXIT_FAILURE);
    }

    currentRow = house.getDockingStationRow();
    currentCol = house.getDockingStationCol();

    wallsSensor = std::make_unique<ConcreteWallsSensor>(house, currentRow, currentCol);
    dirtSensor = std::make_unique<ConcreteDirtSensor>(house, currentRow, currentCol);
    batteryMeter = std::make_unique<ConcreteBatteryMeter>(house.getMaxBattery());
}

void Simulation::setAlgorithm(AbstractAlgorithm& algo) {
    algorithm = &algo;
    algorithm->setMaxSteps(house.getMaxSteps());
    algorithm->setWallsSensor(*wallsSensor);
    algorithm->setDirtSensor(*dirtSensor);
    algorithm->setBatteryMeter(*batteryMeter);
}

void Simulation::run() {
    int steps = 0;
    while (steps < house.getMaxSteps() && batteryMeter->getBatteryState() > 0) {
        Step step = algorithm->nextStep();
        if (step == Step::Finish) {
            break;
        }

        switch (step) {
            case Step::North:
                moveRobot(Direction::North);
                break;
            case Step::East:
                moveRobot(Direction::East);
                break;
            case Step::South:
                moveRobot(Direction::South);
                break;
            case Step::West:
                moveRobot(Direction::West);
                break;
            case Step::Stay:
                // Do nothing
                break;
            case Step::Finish:
                // This case is handled by the if statement above
                break;
        }

        if (dirtSensor->dirtLevel() > 0) {
            house.cleanCell(currentRow, currentCol);
        }

        batteryMeter->useBattery();
        steps++;
    }

    std::cout << "Simulation completed in " << steps << " steps." << std::endl;
}

void Simulation::moveRobot(Direction d) {
    switch (d) {
        case Direction::North:
            if (currentRow > 0 && !wallsSensor->isWall(Direction::North)) currentRow--;
            break;
        case Direction::East:
            if (currentCol < house.getCols() - 1 && !wallsSensor->isWall(Direction::East)) currentCol++;
            break;
        case Direction::South:
            if (currentRow < house.getRows() - 1 && !wallsSensor->isWall(Direction::South)) currentRow++;
            break;
        case Direction::West:
            if (currentCol > 0 && !wallsSensor->isWall(Direction::West)) currentCol--;
            break;
    }
}
