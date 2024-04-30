#include "cameraActor.h"
#include "game.h"
#include "moveComponent.h"
#include "renderer.h"

CameraActor::CameraActor(Game* game )
:Actor(game)
{
    m_movecomp = new MoveComponent(this);

}

void CameraActor::UpdateActor(float deltatime)
{
    Actor::UpdateActor(deltatime);

    Vector3 cameraPos = GetPosition();

    Vector3 targetPos = cameraPos + GetForward() * 1.f;
    auto viewmat = Matrix4::CreateLookAt(
        cameraPos,
        targetPos,
        Vector3::UnitZ);
    GetGame()->GetRenderer()->SetViewMatrix(viewmat);

}

void CameraActor::ActorInput(const uint8_t* states)
{
    float forwardspeed = 0.f;
    float angularSpeed = 0.f;

    if(states[SDL_SCANCODE_W])
    {
        forwardspeed +=100.f;
    }   
    if(states[SDL_SCANCODE_S])
    {
        forwardspeed -=100.f;
    }   
    if(states[SDL_SCANCODE_D])
    {
        angularSpeed +=Math::TwoPi;
    }   
    if(states[SDL_SCANCODE_A])
    {
        angularSpeed -=Math::TwoPi;
    }   
    
    if(states[SDL_SCANCODE_I])
    {
        Vector3 cameraPos = GetPosition();
        SDL_Log("camera pos: %f %f %f ",cameraPos.x,cameraPos.y,cameraPos.z);
        SDL_Log("camera forworad: %f %f %f ",GetForward().x,GetForward().y,GetForward().z);
    };

    m_movecomp->SetForwardSpeed(forwardspeed);
    m_movecomp->SetAngularSpeed(angularSpeed);

}

