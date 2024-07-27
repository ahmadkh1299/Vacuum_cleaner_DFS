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
    void setDockingStation(int Row, int Col);
    Step nextStep() override;

private:
    std::size_t maxSteps;
    const WallsSensor* wallsSensor;
    const DirtSensor* dirtSensor;
    const BatteryMeter* batteryMeter;
    std::stack<std::pair<int, int>> historyStack;
    std::map<std::pair<int, int>, int> visited;
    std::map<std::pair<int, int>, std::vector<Direction>> unexplored;
    bool goingBacktoDoking;
    bool Cleaning;



    int currentRow, currentCol;
    int dockingRow, dockingCol;
    bool isInitialized;

    void initialize(int dockrow, int dockcol);
    Step moveToNextCell();
    Step backtrack();
    Step backtrackToDocking(); // Declaration of the new method
    bool isValidMove(int row, int col);
    void updateUnexplored(int row, int col);
};

#endif // MY_ALGORITHM_H
