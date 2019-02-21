#ifndef MOTOR_H
#define MOTOR_H

#define MIN_REAL_SPEED 138 //115
#define MAX_REAL_SPEED 255
#define REAL_SPEED_RANGE (MAX_REAL_SPEED - MIN_REAL_SPEED)

class Motor
{
public:
    Motor(const byte& pinIn1, const byte& pinIn2, const byte& pinEnable, const bool& invertedPins = false)
    {
        _pinIn1 = pinIn1;
        _pinIn2 = pinIn2;
        _pinEnable = pinEnable;
        _speed = 0;
        _invertedPins = invertedPins;

        pinMode(_pinIn1, OUTPUT);
        pinMode(_pinIn2, OUTPUT);
        pinMode(_pinEnable, OUTPUT);
    }

    ~Motor() {}
    float getSpeed() const
    {
        return _speed;
    }
    bool isInverted() const
    {
        return _invertedPins;
    }
    void setSpeed(const float& speed)
    {
        _speed = constrain(speed, -1, 1);
    }
    void setInverted(const bool& inverted)
    {
        _invertedPins = inverted;
    }
    void updateMovement()
    {
        auto pinA = !_invertedPins ? _pinIn1 : _pinIn2;
        auto pinB = !_invertedPins ? _pinIn2 : _pinIn1;

        if (_speed > 0)
        {
            digitalWrite(pinA, HIGH);
            digitalWrite(pinB, LOW);
        }
        else if (_speed < 0)
        {
            digitalWrite(pinA, LOW);
            digitalWrite(pinB, HIGH);
        }
        else
        {
            digitalWrite(pinA, LOW);
            digitalWrite(pinB, LOW);
        }

        auto absoluteSpeed = abs(_speed);
        auto realSpeed = MIN_REAL_SPEED + trunc(absoluteSpeed * REAL_SPEED_RANGE);
        analogWrite(_pinEnable, realSpeed);
    }

private:
    byte _pinIn1;
    byte _pinIn2;
    byte _pinEnable;
    float _speed;
    bool _invertedPins;
};

#endif
