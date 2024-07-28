#ifndef SIMULATION_H
#define SIMULATION_H

#include "House.h"
#include "ConcreteSensors.h"
#include "abstract_algorithm.h"
#include "Vacuum.h"
#include "ConfigReader.h"
#include <memory>
#include <vector>
#include <string>

class Simulation {
public:
    Simulation();
    bool readHouseFile(const std::string& houseFilePath);
    void setAlgorithm(AbstractAlgorithm& algo);
    void run();
    void writeOutputFile(const std::string& outputFilePath, int finalDirtLeft, const Vacuum& vacuum);

private:
    House house;
    int maxSteps;
    int maxBattery;
    std::unique_ptr<ConcreteWallsSensor> wallsSensor;
    std::unique_ptr<ConcreteDirtSensor> dirtSensor;
    std::unique_ptr<ConcreteBatteryMeter> batteryMeter;
    AbstractAlgorithm* algorithm;
    int currentRow, currentCol;

    void moveRobot(Step d);
    bool isHouseClean() const;
    bool atDockingStation() const;
    int calculateDirtLeft() const;
    std::string stepToString(Step step) const;
};

#endif // SIMULATION_H
