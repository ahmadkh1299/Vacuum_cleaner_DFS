#ifndef CHATGPT_VACUUM_H
#define CHATGPT_VACUUM_H

#include "MyAlgorithm.h"
#include "ConcreteSensors.h"
#include "House.h"
#include <stack>
#include <utility>
#include <vector>
#include <string>

class Vacuum {
public:
    Vacuum(MyAlgorithm& Myalgo, ConcreteWallsSensor& wallsSensor, ConcreteDirtSensor& dirtSensor, ConcreteBatteryMeter& batteryMeter, int max_mission_steps, const House& house);

    Vacuum(AbstractAlgorithm &algorithm, ConcreteWallsSensor &wallsSensor, ConcreteDirtSensor &dirtSensor,
           ConcreteBatteryMeter &batteryMeter, int max_mission_steps, const House &house);

    void outputResults(const std::string& output_file) const;
    void start();

private:
    MyAlgorithm& algorithm;
    ConcreteWallsSensor& wallsSensor;
    ConcreteDirtSensor& dirtSensor;
    ConcreteBatteryMeter& batteryMeter;
    const House& house;
    int max_mission_steps;
    int total_steps;
    std::pair<int, int> current_location;
    std::stack<Step> history;

    std::pair<int, int> getCurrentLocation() const { return current_location; }
    int getX() const { return current_location.first; }
    int getY() const { return current_location.second; }
    void setBatterySteps(int battery) { batteryMeter.useBattery(); }
    void setTotalSteps(int steps) { total_steps = steps; }

    void printPath(const std::stack<Step>& path) const;
    bool move(Step step);
    bool isWall(Direction direction) const;
    void chargeBattery();
    void logStep(Step step);
    bool locatedAtDockingStation() const;
    void update();
    std::vector<std::string> log;

    void updatebat();
};

#endif // CHATGPT_VACUUM_H