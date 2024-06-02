#pragma once

#include "actor.h"

// 理解为环绕展示就对了,想象下blender里的环绕镜头
class OrbitActor:public Actor
{
public:
    OrbitActor(class Game* game);

    void ActorInput(const struct InputState& state ) override;
    
    void SetVisible(bool visible);

private:
    class MeshComponent* m_meshcomp;
    class OrbitCamera*  m_cameracomp;

};