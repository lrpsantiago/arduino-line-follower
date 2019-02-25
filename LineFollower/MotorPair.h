#ifndef MOTOR_PAIR_H
#define MOTOR_PAIR_H

#include "Motor.h"

class MotorPair
{
public:
    MotorPair(const byte& rightPinIn1, const byte& rightPinIn2, const byte& rightPinEnable,
               const byte& leftPinIn1, const byte& leftPinIn2, const byte& leftPinEnable,
               const bool& rightInverted = false, const bool& leftInverted = false)
    {
        _rightMotor = new Motor(rightPinIn1, rightPinIn2, rightPinEnable, rightInverted);
        _leftMotor = new Motor(leftPinIn1, leftPinIn2, leftPinEnable, leftInverted);
        _balance = 0;
    }

    ~MotorPair()
    {
        delete _rightMotor;
        _rightMotor = NULL;

        delete _leftMotor;
        _leftMotor = NULL;
    }

    const Motor* getLeftMotor() const
    {
        return _leftMotor;
    }

    const Motor* getRightMotor() const
    {
        return _rightMotor;
    }

    float getBalance() const
    {
        return _balance;
    }

    void setBalance(const float& balance)
    {
        _balance = constrain(balance, -1, 1);
    }

    void forward(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftMotor->setSpeed(constrainedSpeed * getLeftBalance());
        _rightMotor->setSpeed(constrainedSpeed * getRightBalance());
    }

    void backward(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftMotor->setSpeed(-constrainedSpeed * getLeftBalance());
        _rightMotor->setSpeed(-constrainedSpeed * getRightBalance());
    }

    void turnLeft(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftMotor->setSpeed(0);
        _rightMotor->setSpeed(constrainedSpeed * getRightBalance());
    }

    void turnRight(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftMotor->setSpeed(constrainedSpeed * getLeftBalance());
        _rightMotor->setSpeed(0);
    }

    void rotateLeft(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftMotor->setSpeed(-constrainedSpeed * getLeftBalance());
        _rightMotor->setSpeed(constrainedSpeed * getRightBalance());
    }

    void rotateRight(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftMotor->setSpeed(constrainedSpeed * getLeftBalance());
        _rightMotor->setSpeed(-constrainedSpeed * getRightBalance());
    }

    void stop()
    {
        _leftMotor->setSpeed(0);
        _rightMotor->setSpeed(0);
    }

    void setDirection(const float& direction)
    {        
        auto spdL = direction < 0 ? 1 + direction : 1;
        auto spdR = direction > 0 ? 1 - direction : 1;

        spdL = constrain(spdL, 0, 1);
        spdR = constrain(spdR, 0, 1);

        setIndependentSpeed(spdL, spdR);
    }

    void setIndependentSpeed(const float& leftSpeed = 1, const float& rightSpeed = 1)
    {
        auto constrainedSpeedL = constrainSpeed(leftSpeed);
        auto constrainedSpeedR = constrainSpeed(rightSpeed);

        _leftMotor->setSpeed(constrainedSpeedL * getLeftBalance());
        _rightMotor->setSpeed(constrainedSpeedR * getRightBalance());
    }

    void updateMovement()
    {
        _rightMotor->updateMovement();
        _leftMotor->updateMovement();
    }

private:
    float constrainSpeed(const float& speed)
    {
        auto constrainedSpeed = abs(speed);
        constrainedSpeed = constrain(constrainedSpeed, 0, 1);
        return constrainedSpeed;
    }

    float getLeftBalance()
    {
        return _balance < 0 ? 1 + _balance : 1;
    }

    float getRightBalance()
    {
        return _balance > 0 ? 1 - _balance : 1;
    }

private:
    Motor* _rightMotor;
    Motor* _leftMotor;
    float _balance;
};

#endif
