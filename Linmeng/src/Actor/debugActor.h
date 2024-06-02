#pragma once
#include "actor.h"

class DebugActor:public Actor
{
public:
    DebugActor(class Game* game);
    virtual void ActorInput(const struct InputState& state) override;

};
