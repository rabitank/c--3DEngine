#pragma once
#include "actor.h"

class FollowActor: public Actor{

public:
    FollowActor(class Game* game);

    void ActorInput(const struct InputState& state ) override;
    void SetVisible(bool visible);
    
private:
    class MeshComponent* m_meshcomp;
    class FollowCamera*  m_cameracomp;
    class MoveComponent* m_movecomp;

};