#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H

#include "abstract_algorithm.h"
#include "ConcreteSensors.h"
#include "enums.h"
#include <map>
#include <stack>
#include <utility>

class MyAlgorithm : public AbstractAlgorithm {
public:
    MyAlgorithm();
    void setMaxSteps(std::size_t maxSteps) override;
    void setWallsSensor(const WallsSensor& sensor) override;
    void setDirtSensor(const DirtSensor& sensor) override;
    void setBatteryMeter(const BatteryMeter& meter) override;
    Step nextStep() override;

private:
    std::size_t maxSteps;
    const WallsSensor* wallsSensor;
    const DirtSensor* dirtSensor;
    const BatteryMeter* batteryMeter;
    std::stack<std::pair<int, int>> historyStack;
    std::map<std::pair<int, int>, int> visited; // Stores visited cells and their dirt level
    std::map<std::pair<int, int>, std::vector<Direction>> unexplored; // Stores unexplored cells and their directions

    int currentRow, currentCol;
    bool isInitialized;

    void initialize();
    Step moveToNextCell();
    Step backtrack();
    bool isValidMove(int row, int col);
    void updateUnexplored(int row, int col);
};

#endif // MY_ALGORITHM_H
