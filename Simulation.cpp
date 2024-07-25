#include "Simulation.h"
#include <iostream>
#include <fstream>
#include "Vacuum.h"

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

void Simulation::run() {
    if (!wallsSensor || !dirtSensor || !batteryMeter || !algorithm) {
        std::cerr << "Simulation components not initialized properly." << std::endl;
        return;
    }

    steps = 0;
    while (steps < house.getMaxSteps() && batteryMeter->getBatteryState() > 0) {
        Step nextStep = algorithm->nextStep();
        stepHistory.push_back(nextStep);

        std::cout << "Step " << steps << ": " << stepToString(nextStep) 
                  << " at position (" << currentRow << "," << currentCol << ")" << std::endl;

        if (nextStep == Step::Finish) {
            std::cout << "Algorithm finished cleaning." << std::endl;
            break;
        }

        if (nextStep != Step::Stay) {
            if (!moveRobot(nextStep)) {
                std::cout << "Cannot move in direction: " << static_cast<int>(nextStep) << std::endl;
                continue;
            }
        }

        // Clean the current cell if it has dirt
        int dirtLevel = dirtSensor->dirtLevel();
        if (dirtLevel > 0) {
            house.cleanCell(currentRow, currentCol);
            std::cout << "Cleaned dirt at (" << currentRow << "," << currentCol << ")" << std::endl;
        }

        // Use battery
        batteryMeter->useBattery();

        // Charge if at docking station
        if (atDockingStation()) {
            batteryMeter->chargeBattery(1);  // Charge for 1 step
            std::cout << "Charging at docking station" << std::endl;
        }

        steps++;
    }

    writeOutputFile("simulation_output.txt");
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

bool Simulation::moveRobot(Step step) {
    int newRow = currentRow;
    int newCol = currentCol;

    switch (step) {
        case Step::North: newRow--; break;
        case Step::East:  newCol++; break;
        case Step::South: newRow++; break;
        case Step::West:  newCol--; break;
        case Step::Stay:
        case Step::Finish:
            return true;  // These are valid steps that don't involve movement
        default:
            std::cerr << "Invalid step encountered: " << static_cast<int>(step) << std::endl;
            return false;
    }

    if (newRow < 0 || newRow >= house.getRows() || newCol < 0 || newCol >= house.getCols() || 
        house.getCell(newRow, newCol) == 'W') {
        return false;  // Invalid move
    }

    currentRow = newRow;
    currentCol = newCol;
    wallsSensor->updatePosition(currentRow, currentCol);
    dirtSensor->updatePosition(currentRow, currentCol);
    return true;
}

bool Simulation::isHouseClean() const {
    return house.isHouseClean();
}

bool Simulation::atDockingStation() const {
    return currentRow == house.getDockingStationRow() && currentCol == house.getDockingStationCol();
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