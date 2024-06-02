#include "followActor.h"
#include "game.h"
#include "CameraComp/followCamera.h"
#include "meshComponent.h"
#include "moveComponent.h"
#include "renderer.h"
#include "inputSystem.h"

FollowActor::FollowActor(class Game* game)
    :Actor(game)
{
    m_meshcomp  =  new MeshComponent(this);
    m_meshcomp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));

    SetPosition(Vector3(0.f,0.f,-100.f));
    m_movecomp = new MoveComponent(this);
    
    m_cameracomp = new FollowCamera(this);
    m_cameracomp->SnapToIdeal();
}

void FollowActor::ActorInput(const struct InputState& state)
{
    float forwardSpeed = 0.f;
    float angularSpeed = 0.f;
    const KeyboardState& keyboard = state.keyboard;

    if(keyboard.GetKetValue(SDL_SCANCODE_W))
    {
        forwardSpeed += 400.f;
    }
    if(keyboard.GetKetValue(SDL_SCANCODE_S))
    {
        forwardSpeed -=400.f;
    }
    if(keyboard.GetKetValue(SDL_SCANCODE_D))
    {
        angularSpeed += Math::Pi;
    }
    if(keyboard.GetKetValue(SDL_SCANCODE_A))
    {
        angularSpeed -= Math::Pi;
    }

    m_movecomp->SetForwardSpeed(forwardSpeed);
    m_movecomp->SetAngularSpeed(angularSpeed);

    if(!Math::NearZero(forwardSpeed))
    {
        m_cameracomp->SetHorzDist(500.f);
    }
    else
    {
        m_cameracomp->SetHorzDist(350.f);
    }
    
}

void FollowActor::SetVisible(bool visible)
{
    m_meshcomp->SetVisible(visible);
}

