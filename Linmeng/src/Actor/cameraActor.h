#pragma once

#include "actor.h"
#include "soundEvent.h"

class CameraActor:public Actor
{
public:
    CameraActor(class Game* );

    void UpdateActor(float deltatime) override;
    void ActorInput(const struct InputState& states ) override;

    void SetFootstepSurface(float value);

private:
    class MoveComponent* m_movecomp;

    class AudioComponent* m_audiocomp;
    SoundEvent m_footstep;
    float m_lastFootstep;

};