#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "AbstractState.h"

template<class T>
class StateMachine<T>
{
public:
    StateMachine() {}
    ~StateMachine() {}

    const AbstractState* getCurrentState() const
    {
        return _currentState;
    }

    void setState(AbstractState* state)
    {
        if (_currentState != NULL)
        {
            _currentState->onLeave();
        }

        _currentState = state;

        if (_currentState != NULL)
        {
            _currentState->onEnter();
        }
    }

    void update(const float& delta)
    {
        if (_currentState == NULL)
        {
            return;
        }

        _currentState->update(delta);
    }

private:
    AbstractState<T>* _currentState;
};

#endif
