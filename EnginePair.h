#ifndef ENGINE_PAIR_H
#define ENGINE_PAIR_H

#include "Engine.h"

class EnginePair
{
public:
    EnginePair(const byte& rightPinIn1, const byte& rightPinIn2, const byte& rightPinEnable,
               const byte& leftPinIn1, const byte& leftPinIn2, const byte& leftPinEnable,
               const bool& rightInverted = false, const bool& leftInverted = false)
    {
        _rightEngine = new Engine(rightPinIn1, rightPinIn2, rightPinEnable, rightInverted);
        _leftEngine = new Engine(leftPinIn1, leftPinIn2, leftPinEnable, leftInverted);
        _balance = 0;
    }

    ~EnginePair()
    {
        delete _rightEngine;
        _rightEngine = NULL;

        delete _leftEngine;
        _leftEngine = NULL;
    }

    const Engine* getLeftEngine() const
    {
        return _leftEngine;
    }

    const Engine* getRightEngine() const
    {
        return _rightEngine;
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

        _leftEngine->setSpeed(constrainedSpeed * getLeftBalance());
        _rightEngine->setSpeed(constrainedSpeed * getRightBalance());
    }

    void backward(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftEngine->setSpeed(-constrainedSpeed * getLeftBalance());
        _rightEngine->setSpeed(-constrainedSpeed * getRightBalance());
    }

    void turnLeft(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftEngine->setSpeed(0);
        _rightEngine->setSpeed(constrainedSpeed * getRightBalance());
    }

    void turnRight(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftEngine->setSpeed(constrainedSpeed * getLeftBalance());
        _rightEngine->setSpeed(0);
    }

    void rotateLeft(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftEngine->setSpeed(-constrainedSpeed * getLeftBalance());
        _rightEngine->setSpeed(constrainedSpeed * getRightBalance());
    }

    void rotateRight(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftEngine->setSpeed(constrainedSpeed * getLeftBalance());
        _rightEngine->setSpeed(-constrainedSpeed * getRightBalance());
    }

    void slightTurnLeft(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftEngine->setSpeed(constrainedSpeed * getLeftBalance() * 2 / 3);
        _rightEngine->setSpeed(constrainedSpeed * getRightBalance());
    }

    void slightTurnRight(const float& speed = 1)
    {
        auto constrainedSpeed = constrainSpeed(speed);

        _leftEngine->setSpeed(constrainedSpeed * getLeftBalance());
        _rightEngine->setSpeed(constrainedSpeed * getRightBalance() * 2 / 3);
    }

    void updateMovement()
    {
        _rightEngine->updateMovement();
        _leftEngine->updateMovement();
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
    Engine* _rightEngine;
    Engine* _leftEngine;
    float _balance;
};

#endif
