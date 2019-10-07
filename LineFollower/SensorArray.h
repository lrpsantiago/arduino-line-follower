#ifndef SENSOR_ARRAY_H
#define SENSOR_ARRAY_H

#include "LdrSensor.h"

#define SENSOR_ARRAY_COUNT 5

class SensorArray
{
public:
    SensorArray()
    {
        auto pin = A0;

        for (int i = 0; i < SENSOR_ARRAY_COUNT; i++)
        {
            _sensors[i] = new LdrSensor(pin);
            pin++;
        }
    }

    ~SensorArray()
    {

    }

    void update()
    {
        for (auto s : _sensors)
        {
            s->update();
        }
    }

    void calibrate()
    {
        for (auto s : _sensors)
        {
            s->calibrate();
        }
    }

    void normalizeCalibration()
    {
        
    }

private:
    LdrSensor* _sensors[SENSOR_ARRAY_COUNT];
};

#endif
