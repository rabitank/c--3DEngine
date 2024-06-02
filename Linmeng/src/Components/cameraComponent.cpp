#include "cameraComponent.h"
#include "renderer.h"
#include "actor.h"
#include "audioSystem.h"
#include "game.h"

CameraComponent::CameraComponent(class Actor* owner,int update)
    :Component(owner,update)
{
}

void CameraComponent::SetViewMat(const Matrix4& view)
{
    Game* g = m_owner->GetGame();
    g->GetRenderer()->SetViewMatrix(view);
    g->GetAudioSystem()->SetListener(view);

}

