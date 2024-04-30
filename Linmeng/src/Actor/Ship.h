#pragma once

#include "actor.h"
#include "SDL.h"

class Ship:public Actor{
private:
    float m_RightSpeed;
    float m_DownSpeed ;

    float m_laserCoolDown{0.f};

    class CircleComponent* m_circle;

    float m_rebornCoolDown{0.f};

public:
    Ship(class Game* game);

    void UpdateActor(float deltatiem) override;
    void ActorInput(const uint8_t* states) override;

    float GetRightSpeed() const {return m_RightSpeed;};
    float GetDownSpeed() const {return m_DownSpeed;};

    const CircleComponent& GetCircle() const {return *m_circle;}
};
