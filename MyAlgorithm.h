#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H

#include "abstract_algorithm.h"
#include "ConcreteSensors.h"
#include "enums.h"
#include <stack>
#include <vector>

enum Color { WHITE, GREY, BLACK };

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
    std::vector<std::vector<Color>> colorMatrix;

    int currentRow, currentCol;
    bool isInitialized;

    void initializeColorMatrix(int rows, int cols);
    Step moveToNextCell();
    bool isValidMove(int row, int col);
    bool allNeighborsBlack(int row, int col);
};

#endif // MY_ALGORITHM_H
