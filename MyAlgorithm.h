#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H

#include "abstract_algorithm.h"
#include <map>
#include <stack>
#include <vector>
#include <utility>
#include <set>

class MyAlgorithm : public AbstractAlgorithm {
public:
    MyAlgorithm();
    void setDockingStation(int row, int col);
    void setMaxSteps(std::size_t maxSteps) override;
    void setWallsSensor(const WallsSensor& sensor) override;
    void setDirtSensor(const DirtSensor& sensor) override;
    void setBatteryMeter(const BatteryMeter& meter) override;
    Step nextStep() override;
    void updatePosition(int row, int col);
    void resetExploration();



private:
    std::size_t maxSteps;
    std::set<std::pair<int, int>> cleanedCells;
    const WallsSensor* wallsSensor;
    const DirtSensor* dirtSensor;
    const BatteryMeter* batteryMeter;
    int currentRow, currentCol;
    int dockingRow, dockingCol;
    bool isInitialized;
    std::map<std::pair<int, int>, int> visited;
    std::map<std::pair<int, int>, std::vector<Direction>> unexplored;
    std::stack<std::pair<int, int>> historyStack;

    void initialize(int dockrow, int dockcol);
    Step moveToNextCell();
    bool isValidMove(int row, int col);
    void updateUnexplored(int row, int col);
    std::vector<Step> findPathToDock();
    Direction directionFromOffset(const std::pair<int, int>& offset);
    std::vector<Step> reconstructPath(
        const std::map<std::pair<int, int>, std::pair<int, int>>& cameFrom,
        std::pair<int, int> current);
};

#endif // MY_ALGORITHM_H