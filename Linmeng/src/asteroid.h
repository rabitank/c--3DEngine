
#pragma once
#include "actor.h"

class Asteroid : public Actor
{
    class CircleComponent* m_circle;

public:
    Asteroid(class Game* ins);

    const CircleComponent& GetCircle() const { return *m_circle;}
    

};
