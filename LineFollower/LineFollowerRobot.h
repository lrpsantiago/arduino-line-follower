#ifndef LINE_FOLLOWER_TOBOT_H
#define LINE_FOLLOWER_TOBOT_H

class LineFollowerRobot : StateMachine
{
public:
    LineFollowerRobot()
        : StateMachine()
    {
        _calibratingState = new CalibratingState(this);
        _normalState = new NormalState(this);
        _turningState = new TurningState(this);
    }

    ~LineFollowerRobot() {}

private:
    AbstractMachine* _calibratingState;
    AbstractMachine* _normalState;
    AbstractMachine* _turningState;

    EnginePair* _engines;
};

#endif
