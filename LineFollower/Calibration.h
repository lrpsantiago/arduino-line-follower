#ifndef CALIBRATION_H
#define CALIBRATION_H

class Calibration
{
public:
    Calibration()
    {
        reset();
    }

    ~Calibration() {}

    int getMin() const
    {
        return _min;
    }

    int getMax() const
    {
        return _max;
    }

    int getRange() const
    {
        return _max - _min;
    }

    int getNormalizedValue(const float& percentage) const
    {
        return _min + trunc((float)getRange() * percentage);
    }

    void setMin(const int& value)
    {
        if (value >= _min)
        {
            return;
        }

        _min = value;
    }

    void setMax(const int& value)
    {
        if (value <= _max)
        {
            return;
        }

        _max = value;
    }

    void reset()
    {
        _min = 1023;
        _max = 0;
    }

private:
    int _min;
    int _max;
};

#endif
