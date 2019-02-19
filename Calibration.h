#ifndef CALIBRATION_H
#define CALIBRATION_H

class Calibration
{
public:
    Calibration()
    {
        _min = 0;
        _max = 0;
    }

    ~Calibration() {}
    byte getMin() const
    {
        return _min;
    }
    byte getMax() const
    {
        return _max;
    }
    byte getRange() const
    {
        return _max - _min;
    }
    byte getNormalizedValue(const float& percentage) const
    {
        return _min + trunc((float)getRange() * percentage);
    }
    void setMin(const byte& value)
    {
        _min = value;
    }
    void setMax(const byte& value)
    {
        _max = value;
    }

private:
    byte _min;
    byte _max;
};

#endif
