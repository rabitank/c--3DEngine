#pragma once

#include "actor.h"


class CameraActor:public Actor
{
public:
    CameraActor(class Game* );

    void UpdateActor(float deltatime) override;
    void ActorInput(const uint8_t* states ) override;
private:
    class MoveComponent* m_movecomp;

};