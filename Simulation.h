#ifndef SIMULATION_H
#define SIMULATION_H

#include "House.h"
#include "ConcreteSensors.h"
#include "abstract_algorithm.h"
#include <memory>

class Simulation {
public:
    Simulation();
    void readHouseFile(const std::string& houseFilePath);
    void setAlgorithm(AbstractAlgorithm& algo);
    void run();

private:
    House house;
    std::unique_ptr<ConcreteWallsSensor> wallsSensor;
    std::unique_ptr<ConcreteDirtSensor> dirtSensor;
    std::unique_ptr<ConcreteBatteryMeter> batteryMeter;
    AbstractAlgorithm* algorithm;
    int currentRow, currentCol;

    void moveRobot(Direction d);
};

#endif // SIMULATION_H
