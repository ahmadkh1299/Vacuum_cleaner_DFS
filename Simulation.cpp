#include "Simulation.h"
#include <iostream>
#include <fstream>

Simulation::Simulation()
        : house(""), currentRow(0), currentCol(0), algorithm(nullptr), steps(0) {}

bool Simulation::readHouseFile(const std::string& houseFilePath) {
    house = House(houseFilePath);
    if (!house.isValid()) {
        std::cerr << "Invalid house file." << std::endl;
        return false;
    }

    currentRow = house.getDockingStationRow();
    currentCol = house.getDockingStationCol();

    wallsSensor = std::make_unique<ConcreteWallsSensor>(house, currentRow, currentCol);
    dirtSensor = std::make_unique<ConcreteDirtSensor>(house, currentRow, currentCol);
    batteryMeter = std::make_unique<ConcreteBatteryMeter>(house.getMaxBattery());

    return true;
}

void Simulation::setAlgorithm(AbstractAlgorithm& algo) {
    algorithm = &algo;
    algorithm->setMaxSteps(house.getMaxSteps());
    algorithm->setWallsSensor(*wallsSensor);
    algorithm->setDirtSensor(*dirtSensor);
    algorithm->setBatteryMeter(*batteryMeter);
}

bool Simulation::isHouseClean() const {
    return house.isHouseClean();
}

bool Simulation::atDockingStation() const {
    return currentRow == house.getDockingStationRow() && currentCol == house.getDockingStationCol();
}

void Simulation::run() {
    printf("%d , %zu",house.getMaxSteps(), batteryMeter->getBatteryState());
    while (steps < house.getMaxSteps() && batteryMeter->getBatteryState() > 0) {
        if (isHouseClean() && atDockingStation()) {
            stepHistory.push_back(Step::Finish);
            std::cout << "Algorithm finished: house is clean and robot is at the docking station." << std::endl;
            break;
        }

        Step step = algorithm->nextStep();
        stepHistory.push_back(step);

        // Print each step
        std::cout << "Step " << steps + 1 << ": " << stepToString(step) << std::endl;

        if (step == Step::Finish) {
            std::cout << "Algorithm requested finish." << std::endl;
            break;
        }
        else if (step == Step::Stay) {
            if (dirtSensor->dirtLevel() > 0) {
                house.cleanCell(currentRow, currentCol);
            } else if (atDockingStation()) {
                batteryMeter->chargeBattery(1);
            }
            break;
        } else {
            moveRobot(step);
        }
        batteryMeter->useBattery();
        steps++;
    }

    std::cout << "Simulation completed in " << steps << " steps." << std::endl;
}

void Simulation::moveRobot(Step d) {
    switch (d) {
        case Step::North:
            if (currentRow > 0) currentRow--;
            break;
        case Step::East:
            if (currentCol < house.getCols() - 1) currentCol++;
            break;
        case Step::South:
            if (currentRow < house.getRows() - 1) currentRow++;
            break;
        case Step::West:
            if (currentCol > 0) currentCol--;
            break;
        case Step::Stay:
            break;
        case Step::Finish:
            break;
    }
    // Update the position of the sensors after the move
    wallsSensor->updatePosition(currentRow, currentCol);
    dirtSensor->updatePosition(currentRow, currentCol);
}

int Simulation::calculateDirtLeft() const {
    int totalDirt = 0;
    for (int i = 0; i < house.getRows(); ++i) {
        for (int j = 0; j < house.getCols(); ++j) {
            char cell = house.getCell(i, j);
            if (cell >= '1' && cell <= '9') {
                totalDirt += cell - '0';
            }
        }
    }
    return totalDirt;
}

std::string Simulation::stepToString(Step step) const {
    switch (step) {
        case Step::North: return "N";
        case Step::East: return "E";
        case Step::South: return "S";
        case Step::West: return "W";
        case Step::Stay: return "s";
        case Step::Finish: return "F";
        default: return "";
    }
}

void Simulation::writeOutputFile(const std::string& outputFilePath) {
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
        return;
    }

    int dirtLeft = calculateDirtLeft();
    std::string status = (steps < house.getMaxSteps() && batteryMeter->getBatteryState() > 0) ? "FINISHED" : "WORKING";

    outputFile << "NumSteps = " << steps << "\n";
    outputFile << "DirtLeft = " << dirtLeft << "\n";
    outputFile << "Status = " << status << "\n";
    outputFile << "Steps:\n";

    for (const Step& step : stepHistory) {
        outputFile << stepToString(step);
    }
    outputFile.close();
}
