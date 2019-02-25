#ifndef LED_H
#define LED_H

#include "ElectronicComponent.h"

class Led : public ElectronicComponent
{
public:
    Led(const byte& pin) : ElectronicComponent(pin, OUTPUT)
    {

    }

    bool isOn()
    {
        return bool(digitalRead(getPin()));
    }

    void toggle()
    {
        if (isOn())
        {
            turnOff();
        }
        else
        {
            turnOn();
        }
    }

    void turnOn()
    {
        digitalWrite(getPin(), HIGH);
    }

    void turnOff()
    {
        digitalWrite(getPin(), LOW);
    }
};

#endif
