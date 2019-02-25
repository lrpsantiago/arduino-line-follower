#ifndef ELECTRONIC_COMPONENT_H
#define ELECTRONIC_COMPONENT_H

class ElectronicComponent
{
public:
    ElectronicComponent(const byte& pin, const byte& mode)
    {
        _pin = pin;
        pinMode(getPin(), mode);
    }

    byte getPin() const
    {
        return _pin;
    }

private:
    byte _pin;
};

#endif
