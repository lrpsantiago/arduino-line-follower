#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PidController
{
public:
    PidController(const int& kp, const int& ki, const int& kd)
        : _kp(kp), _ki(ki), _kd(kd)
    {
        reset();
    }

    float calculatePid(const float& error)
    {
        _errorSum += error;

        auto proportional = error;
        auto integral = _errorSum;
        auto derivative = error - _lastError;
        auto pid = (_kp * proportional) + (_ki * integral) + (_kd * derivative);

        _lastError = error;

        return pid;
    }

    void reset()
    {
        _errorSum = 0;
        _lastError = 0;
    }

private:
    const int _kp;
    const int _ki;
    const int _kd;
    float _lastError;
    float _errorSum;
};

#endif
