#ifndef CONCRETE_SENSORS_H
#define CONCRETE_SENSORS_H

#include "wall_sensor.h"
#include "dirt_sensor.h"
#include "battery_meter.h"
#include "House.h"

class ConcreteWallsSensor : public WallsSensor {
public:
    ConcreteWallsSensor(const House& house, int row, int col);
    bool isWall(Direction d) const override;
    void updatePosition(int row, int col);

private:
    const House& house;
    int row, col;
};

class ConcreteDirtSensor : public DirtSensor {
public:
    ConcreteDirtSensor(const House& house, int row, int col);
    int dirtLevel() const override;
    void updatePosition(int row, int col);

private:
    const House& house;
    int row, col;
};

class ConcreteBatteryMeter : public BatteryMeter {
public:
    ConcreteBatteryMeter(int maxBattery);
    std::size_t getBatteryState() const override;
    void useBattery();
    void chargeBattery(int stepsStayedAtDock);
    int getmaxBattery() const;

private:
    int maxBattery;
    double currentBattery;

};

#endif // CONCRETE_SENSORS_H
