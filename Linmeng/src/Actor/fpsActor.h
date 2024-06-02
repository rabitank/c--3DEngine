#pragma once
#include "actor.h"
#include "soundEvent.h"

class FPSActor:public Actor
{
public:
    FPSActor(class Game* game);
    void UpdateActor(float deltatime) override;
    void ActorInput(const struct InputState& state ) override;

    void SetFootstepSurface(float mix );
    void SetVisible(bool isVisiable) ;

private:
    class MeshComponent* m_meshcomp;
    class FPSCamera*     m_cameracomp;
    class AudioComponent*m_audiocomp;
    class MoveComponent* m_movecomp;
    class Actor* m_modelA;

    SoundEvent m_stepSound;
    /// @brief 一次播放中 , 剩余的脚步声时间
    float m_lastFootstep;

};
