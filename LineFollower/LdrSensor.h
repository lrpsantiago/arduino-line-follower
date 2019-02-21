#ifndef LDR_SENSOR_H
#define LDR_SENSOR_H

#include "Calibration.h"

class LdrSensor
{
public:
    LdrSensor(const byte& pin)
    {
        _pin = pin;
        _calibration = new Calibration();

        pinMode(_pin, INPUT);
    }

    ~LdrSensor()
    {
        delete _calibration;
        _calibration = NULL;
    }

    int getValue() const
    {
        return analogRead(_pin);
    }

    float getNormalizedValue() const
    {
        auto value = analogRead(_pin);
        auto normalizedValue = float(value - _calibration->getMin()) / _calibration->getRange();
        
        return constrain(normalizedValue, 0, 1);
    }

    void calibrate(const int& value)
    {
        _calibration->setMin(value);
        _calibration->setMax(value);
    }

private:
    byte _pin;
    Calibration* _calibration;
};

#endif
