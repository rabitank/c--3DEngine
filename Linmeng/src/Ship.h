#pragma once

#include "actor.h"
#include "SDL.h"

class Ship:public Actor{
private:
    float m_RightSpeed;
    float m_DownSpeed ;
public:
    Ship(Game* game);

    void UpdateActor(float deltatiem) override;
    void ProccessKeyboard( const uint8_t *state);
    float GetRightSpeed() const {return m_RightSpeed;};
    float GetDownSpeed() const {return m_DownSpeed;};

};
