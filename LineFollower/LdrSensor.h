#ifndef LDR_SENSOR_H
#define LDR_SENSOR_H

#include "Calibration.h"

#define SENSOR_VALUES_COUNT 5

class LdrSensor
{
public:
    LdrSensor(const byte& pin)
    {
        _pin = pin;
        _calibration = new Calibration();
        _previousValues = new int[SENSOR_VALUES_COUNT];
        _smoothedValue = 0;
        
        for (int i = 0; i < SENSOR_VALUES_COUNT; i++)
        {
            _previousValues[i] = 0;
        }

        pinMode(_pin, INPUT);
    }

    ~LdrSensor()
    {
        delete _calibration;
        _calibration = NULL;
    }

    void update()
    {
        auto value = analogRead(_pin);

        for (int i = SENSOR_VALUES_COUNT - 1; i > 0; i--)
        {
            _previousValues[i] = _previousValues[i - 1];
        }

        _previousValues[0] = value;
        _smoothedValue = wma();
    }

    int getRawValue() const
    {
        return analogRead(_pin);
    }

    int getSmoothedValue() const
    {
        return _smoothedValue;
    }

    Calibration* getCalibration() const
    {
        return _calibration;
    }

    float getNormalizedValue() const
    {
        auto value = _smoothedValue;
        auto normalizedValue = float(value - _calibration->getMin()) / _calibration->getRange();
        
        return constrain(normalizedValue, 0, 1);
    }

    void calibrate()
    {
        auto rawValue = getRawValue();
        _calibration->calibrate(rawValue);
    }

private:
    int wma()
    {
        const int quo = (SENSOR_VALUES_COUNT + 1) * SENSOR_VALUES_COUNT / 2;
        float sum = 0;
        
        for (int i = 0; i < SENSOR_VALUES_COUNT; i++)
        {
            sum += _previousValues[i] * (SENSOR_VALUES_COUNT - i);
        }

        return sum / quo;
    }

private:
    byte _pin;
    Calibration* _calibration;
    int* _previousValues;
    int _smoothedValue;
};

#endif
