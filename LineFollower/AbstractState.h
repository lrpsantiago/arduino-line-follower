#ifndef ABSTRACT_STATE_H
#define ABSTRACT_STATE_H

#include "StateMachine.h"

class AbstractState
{
public:
    StateMachine(StateMachine* owner)
    {
        _owner = owner;
    }

    virtual ~StateMachine()
    {
        _owner = NULL;
    }

    virtual void onEnter() {}
    virtual void update(const float& delta) = 0;
    virtual void onLeave() {}

private:
    StateMachine* _owner;
};

#endif
