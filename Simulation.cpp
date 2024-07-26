/*
#include "Simulation.h"
#include "ConfigReader.h"
#include <iostream>
#include <fstream>

Simulation::Simulation()
        : house(), maxSteps(0), maxBattery(0), currentRow(0), currentCol(0), algorithm(nullptr), steps(0) {}

bool Simulation::readHouseFile(const std::string& houseFilePath) {
    try {
        ConfigReader config(houseFilePath);
        maxSteps = config.getMaxSteps();
        maxBattery = config.getMaxBattery();
        house = House(config.getLayout());


        currentRow = house.getDockingStationRow();
        currentCol = house.getDockingStationCol();

        wallsSensor = std::make_unique<ConcreteWallsSensor>(house, currentRow, currentCol);
        dirtSensor = std::make_unique<ConcreteDirtSensor>(house, currentRow, currentCol);
        batteryMeter = std::make_unique<ConcreteBatteryMeter>(maxBattery);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Simulation::setAlgorithm(AbstractAlgorithm& algo) {
    algorithm = &algo;
    algorithm->setMaxSteps(maxSteps);
    algorithm->setWallsSensor(*wallsSensor);
    algorithm->setDirtSensor(*dirtSensor);
    algorithm->setBatteryMeter(*batteryMeter);

    // Set docking station if the algorithm supports it
    MyAlgorithm* myAlgo = dynamic_cast<MyAlgorithm*>(algorithm);
    if (myAlgo) {
        myAlgo->setDockingStation(currentRow, currentCol);
    }
}

void Simulation::run() {
    if (!wallsSensor || !dirtSensor || !batteryMeter || !algorithm) {
        std::cerr << "Simulation components not initialized properly." << std::endl;
        return;
    }

    Vacuum vacuum(*algorithm, *wallsSensor, *dirtSensor, *batteryMeter, maxSteps, house);
    vacuum.start();
    vacuum.outputResults("simulation_output.txt");
}


void Simulation::writeOutputFile(const std::string& outputFilePath) {
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
        return;
    }

    int dirtLeft = calculateDirtLeft();
    std::string status = (steps < maxSteps && batteryMeter->getBatteryState() > 0) ? "FINISHED" : "WORKING";

    outputFile << "NumSteps = " << steps << "\n";
    outputFile << "DirtLeft = " << dirtLeft << "\n";
    outputFile << "Status = " << status << "\n";
    outputFile << "Steps:\n";

    for (const Step& step : stepHistory) {
        outputFile << stepToString(step);
    }
    outputFile.close();
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
    wallsSensor->updatePosition(currentRow, currentCol);
    dirtSensor->updatePosition(currentRow, currentCol);
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
            int cell = house.getCell(i, j);
            if (cell > 0) {
                totalDirt += cell;
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
*/
#include "Simulation.h"
#include "ConfigReader.h"
#include <iostream>
#include <fstream>

Simulation::Simulation()
        : house(), maxSteps(0), maxBattery(0), currentRow(0), currentCol(0), algorithm(nullptr), steps(0) {}

bool Simulation::readHouseFile(const std::string& houseFilePath) {
    try {
        ConfigReader config(houseFilePath);
        maxSteps = config.getMaxSteps();
        maxBattery = config.getMaxBattery();
        house = House(config.getLayout());
        currentRow = house.getDockingStationRow();
        currentCol = house.getDockingStationCol();

        wallsSensor = std::make_unique<ConcreteWallsSensor>(house, currentRow, currentCol);
        dirtSensor = std::make_unique<ConcreteDirtSensor>(house, currentRow, currentCol);
        batteryMeter = std::make_unique<ConcreteBatteryMeter>(maxBattery);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Simulation::setAlgorithm(AbstractAlgorithm& algo) {
    algorithm = &algo;
    algorithm->setMaxSteps(maxSteps);
    algorithm->setWallsSensor(*wallsSensor);
    algorithm->setDirtSensor(*dirtSensor);
    algorithm->setBatteryMeter(*batteryMeter);

    MyAlgorithm* myAlgo = dynamic_cast<MyAlgorithm*>(algorithm);
    if (myAlgo) {
        myAlgo->setDockingStation(currentRow, currentCol);
    }
}

void Simulation::run() {
    if (!wallsSensor || !dirtSensor || !batteryMeter || !algorithm) {
        std::cerr << "Simulation components not initialized properly." << std::endl;
        return;
    }

    Vacuum vacuum(*algorithm, *wallsSensor, *dirtSensor, *batteryMeter, maxSteps, house);
    vacuum.start();
    vacuum.outputResults("C:\\Users\\97250\\Desktop\\TAU\\5th\\Advanced Programing\\Vacuum_cleaner_DFS\\simulation_output.txt");
}

void Simulation::writeOutputFile(const std::string& outputFilePath) {
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
        return;
    }

    int dirtLeft = calculateDirtLeft();
    std::string status = (steps < maxSteps && batteryMeter->getBatteryState() > 0) ? "FINISHED" : "WORKING";

    outputFile << "NumSteps = " << steps << "\n";
    outputFile << "DirtLeft = " << dirtLeft << "\n";
    outputFile << "Status = " << status << "\n";
    outputFile << "Steps:\n";

    for (const Step& step : stepHistory) {
        outputFile << stepToString(step);
    }
    outputFile.close();
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
    wallsSensor->updatePosition(currentRow, currentCol);
    dirtSensor->updatePosition(currentRow, currentCol);
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
            int cell = house.getCell(i, j);
            if (cell > 0) {
                totalDirt += cell;
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
