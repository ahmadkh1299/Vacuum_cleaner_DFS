#include "Vacuum.h"
#include <iostream>
#include <fstream>

Vacuum::Vacuum(MyAlgorithm& algorithm, ConcreteWallsSensor& wallsSensor, ConcreteDirtSensor& dirtSensor, ConcreteBatteryMeter& batteryMeter, int max_mission_steps,House& house)
        : algorithm(algorithm), wallsSensor(wallsSensor), dirtSensor(dirtSensor), batteryMeter(batteryMeter),
          max_mission_steps(max_mission_steps), total_steps(0), current_location(0, 0), house(house) {}

/*
void Vacuum::start() {
    current_location = std::make_pair(house.getDockingStationRow(), house.getDockingStationCol());
    while (total_steps < max_mission_steps && batteryMeter.getBatteryState() > 0) {
        Step nextStep = algorithm.nextStep();
        logStep(nextStep);

        if (nextStep == Step::Stay) {
            int dirtLevel = dirtSensor.dirtLevel();
            if (dirtLevel > 0) {
                house.cleanCell(current_location.first, current_location.second);
                std::cout << "Cleaned cell at (" << current_location.first << ", " << current_location.second
                          << "). New dirt level: " << dirtSensor.dirtLevel() << std::endl;
            }
        } else if (nextStep == Step::Finish) {
            if (!house.isHouseClean()) {
                // If house is not clean, continue exploration
                std::cout << "House not fully clean. Continuing exploration." << std::endl;
                algorithm.resetExploration();
                continue;
            }
            std::cout << "House is clean. Returning to docking station." << std::endl;
            break;
        } else if (!move(nextStep)) {
            std::cout << "Cannot move in direction: " << static_cast<int>(nextStep) << std::endl;
            break;
        }

        updatebat(); // update battery steps
        total_steps++; // update total steps
        wallsSensor.updatePosition(current_location.first, current_location.second);
        dirtSensor.updatePosition(current_location.first, current_location.second);

        if (house.isHouseClean()) {
            std::cout << "House is clean. Finishing." << std::endl;
            break;
        }
    }
}*/
void Vacuum::start() {
    current_location = std::make_pair(house.getDockingStationRow(), house.getDockingStationCol());
    history = std::stack<Step>();
    std::stack<Step> path_back_to_last_cleaned;

    while (total_steps < max_mission_steps && batteryMeter.getBatteryState() > 0) {
        // Check if we need to return to the docking station
        if ((batteryMeter.getBatteryState() <= history.size()) ||(max_mission_steps-total_steps <= batteryMeter.getBatteryState()) ) {
            std::stack<Step> path_to_docking = algorithm.findPathToDocking(history);
            while (!path_to_docking.empty() && total_steps < max_mission_steps) {
                move(path_to_docking.top());
                path_to_docking.pop();
                wallsSensor.updatePosition(current_location.first, current_location.second);
                dirtSensor.updatePosition(current_location.first, current_location.second);
                std::cout << "Returning to docking station. Current location: ("
                          << current_location.first << ", " << current_location.second << "), "
                          << "Battery level: " << batteryMeter.getBatteryState() << std::endl;
                if (!locatedAtDockingStation()) {
                    update();
                }
            }
            if (locatedAtDockingStation()) {
                chargeBattery();
                history = std::stack<Step>(); // Clear the history after recharging
            }

            // Return to the last cleaned position after charging
            if (total_steps < max_mission_steps && !path_back_to_last_cleaned.empty()) {
                while (!path_back_to_last_cleaned.empty() && total_steps < max_mission_steps) {
                    move(path_back_to_last_cleaned.top());
                    path_back_to_last_cleaned.pop();
                    update();
                    wallsSensor.updatePosition(current_location.first, current_location.second);
                    dirtSensor.updatePosition(current_location.first, current_location.second);
                }
            }
        } else {
            Step nextStep = algorithm.nextStep();
            logStep(nextStep);
            if (nextStep == Step::Stay) {
                int dirtLevel = dirtSensor.dirtLevel();
                if (dirtLevel > 0) {
                    house.cleanCell(current_location.first, current_location.second);
                    updatebat();
                }
            } else if (nextStep == Step::Finish) {
                if (!house.isHouseClean()) {
                    algorithm.resetExploration();
                    continue;
                }
                break;
            } else if (!move(nextStep)) {
                break;
            }

            // Record path for returning to the last cleaned cell after charging
            path_back_to_last_cleaned = history;
            total_steps++;
            updatebat();
            wallsSensor.updatePosition(current_location.first, current_location.second);
            dirtSensor.updatePosition(current_location.first, current_location.second);

            if (house.isHouseClean()) {
                break;
            }
        }
    }
}




void Vacuum::outputResults(const std::string& output_file) const {
    std::ofstream file(output_file);
    if (!file.is_open()) {
        std::cerr << "Failed to open output file: " << output_file << std::endl;
        return;
    }

    int dirtLeft = house.calculateTotalDirt();
    std::string status;
    if (batteryMeter.getBatteryState() <= 0) {
        status = "DEAD";
    } else if (house.isHouseClean()) {
        status = "FINISHED";
    } else {
        status = "WORKING";
    }

    int numSteps = total_steps;
    if (!log.empty() && log.back().back() == 'F') {
        numSteps--;
    }

    file << "NumSteps = " << numSteps << "\n";
    file << "DirtLeft = " << dirtLeft << "\n";
    file << "Status = " << status << "\n";
    file << "Steps:\n";

    for (const auto& logEntry : log) {
        file << logEntry;
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
        if (step!= Step::Stay){
            history.push(step);
        }
        return true;
}

    bool Vacuum::isWall(Direction direction) const {
        return wallsSensor.isWall(direction);
    }


void Vacuum::chargeBattery() {
    std::cout << "start charging, current battery: " << batteryMeter.getBatteryState() << std::endl;
    while (batteryMeter.getBatteryState() < batteryMeter.getmaxBattery() && total_steps < max_mission_steps) {
        batteryMeter.chargeBattery(1); // Charge the battery by 1 unit
        total_steps++; // Increment total steps
        std::cout << "charging, current battery: " << batteryMeter.getBatteryState() << std::endl;
        std::cout << "total steps: " << total_steps << std::endl;
    }
    std::cout << "battery done charging, current battery: " << batteryMeter.getBatteryState() << std::endl;
    std::cout << "total steps after charging: " << total_steps << std::endl;
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
    void Vacuum::update() {
        batteryMeter.useBattery();
        total_steps++;
    }
    void Vacuum::updatebat() { batteryMeter.useBattery(); }