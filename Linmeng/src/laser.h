#pragma once

#include "actor.h"

class Laser: public Actor
{
    class CircleComponent* m_circle;
    float m_deathTimer{1.f};

public:
    Laser(Game* ins);
    void UpdateActor(float deltatime) override;

};
