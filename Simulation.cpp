#include "Simulation.h"
#include <iostream>
#include <fstream>

Simulation::Simulation()
        : house(""), currentRow(0), currentCol(0), algorithm(nullptr), steps(0) {}

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
    while (steps < house.getMaxSteps() && batteryMeter->getBatteryState() > 0) {
        Step step = algorithm->nextStep();
        stepHistory.push_back(step);
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
