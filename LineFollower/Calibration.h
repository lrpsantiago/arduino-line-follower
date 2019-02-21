#ifndef CALIBRATION_H
#define CALIBRATION_H

class Calibration
{
public:
    Calibration()
    {
        _min = 1024;
        _max = 0;
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

private:
    int _min;
    int _max;
};

#endif
