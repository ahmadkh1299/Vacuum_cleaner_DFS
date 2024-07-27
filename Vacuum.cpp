#include "Vacuum.h"
#include <iostream>
#include <fstream>

Vacuum::Vacuum(MyAlgorithm& algorithm, ConcreteWallsSensor& wallsSensor, ConcreteDirtSensor& dirtSensor, ConcreteBatteryMeter& batteryMeter, int max_mission_steps, const House& house)
        : algorithm(algorithm), wallsSensor(wallsSensor), dirtSensor(dirtSensor), batteryMeter(batteryMeter),
          max_mission_steps(max_mission_steps), total_steps(0), current_location(0, 0), house(house) {}

void Vacuum::start() {
    current_location = std::make_pair(house.getDockingStationRow(), house.getDockingStationCol());
    while (total_steps < max_mission_steps && batteryMeter.getBatteryState() > 0) {
        if (batteryMeter.getBatteryState() <= history.size()+2) { //need to charge
            std::stack<Step> path_to_docking = algorithm.findPathToDocking(history); //take the next steps to docking station
            while ((!path_to_docking.empty())  && total_steps<max_mission_steps) {
                move(path_to_docking.top());
                path_to_docking.pop();
                if (!locatedAtDockingStation()) {
                    update(); //update battery and steps
                }
            }
            if (locatedAtDockingStation()) {
                chargeBattery(); //
                history = std::stack<Step>();
            }
            // Clear history after recharging
            // continue;
        }


            Step nextStep = algorithm.nextStep();
            logStep(nextStep);
            if (!move(nextStep)) {
                std::cout << "Cannot move in direction: " << static_cast<int>(nextStep) << std::endl;
                break;
            }
            printf("%d ,%d\n",current_location.first, current_location.second);
            total_steps++;
            wallsSensor.updatePosition(current_location.first, current_location.second);
            dirtSensor.updatePosition(current_location.first, current_location.second);
        }
    }
}

    void Vacuum::outputResults(const std::string& output_file) const {
        std::ofstream file(output_file);
        if (!file.is_open()) {
            std::cerr << "Failed to open output file." << std::endl;
            return;
        }
        file << "Total steps: " << total_steps << "\n";
        for (const auto& logEntry : log) {
            file << logEntry << "\n";
        }
        file.close();
    }

    bool Vacuum::move(Step step) {
        Direction direction = static_cast<Direction>(step);

        if (step!=Step::Stay && isWall(direction)) {
            return false;
        }
        switch (step) {
            case Step::North:
                current_location.first--;
                break;
            case Step::East:
                current_location.second++;
                break;
            case Step::South:
                current_location.first++;
                break;
            case Step::West:
                current_location.second--;
                break;
            case Step::Stay:
            case Step::Finish:
                break;
        }
        batteryMeter.useBattery();
        return true;
    }

    bool Vacuum::isWall(Direction direction) const {
        return wallsSensor.isWall(direction);
    }


void Vacuum::chargeBattery() { // charging
    while (batteryMeter.getBatteryState() < batteryMeter.getmaxBattery() && total_steps < max_mission_steps) {
        batteryMeter.chargeBattery(1);
        total_steps++;
    }
}

    void Vacuum::logStep(Step step) {
        std::string stepStr;
        switch (step) {
            case Step::North: stepStr = "N"; break;
            case Step::East: stepStr = "E"; break;
            case Step::South: stepStr = "S"; break;
            case Step::West: stepStr = "W"; break;
            case Step::Stay: stepStr = "s"; break;
            case Step::Finish: stepStr = "F"; break;
        }
        log.push_back("Step " + std::to_string(total_steps) + ": " + stepStr);
    }

    bool Vacuum::locatedAtDockingStation() const {
        return current_location == std::make_pair(house.getDockingStationRow(), house.getDockingStationCol());
    }