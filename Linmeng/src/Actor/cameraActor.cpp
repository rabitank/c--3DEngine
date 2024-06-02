#include "cameraActor.h"
#include "game.h"
#include "moveComponent.h"
#include "renderer.h"
#include "audioComponent.h"
#include "audioSystem.h"
#include "inputSystem.h"

CameraActor::CameraActor(Game* game )
:Actor(game)
{
    m_movecomp = new MoveComponent(this);
    m_audiocomp = new AudioComponent(this);
    m_lastFootstep = 0.f;
    m_footstep = m_audiocomp->PlayEvent("event:/Footstep");
    m_footstep.SetPaused(true);
}

void CameraActor::UpdateActor(float deltatime)
{
    Actor::UpdateActor(deltatime);
    m_lastFootstep -=deltatime;
    if(!Math::NearZero(m_movecomp->GetForwardSpeed()) && m_lastFootstep <=0.f)
    {
        m_footstep.SetPaused(false);
        m_footstep.Resart();
        m_lastFootstep = 0.5f;
    }

    Vector3 cameraPos = GetPosition();

    Vector3 targetPos = cameraPos + GetForward() * 1.f;
    auto viewmat = Matrix4::CreateLookAt(
        cameraPos,
        targetPos,
        Vector3::UnitZ);
    GetGame()->GetRenderer()->SetViewMatrix(viewmat);
    GetGame()->GetAudioSystem()->SetListener(viewmat);
}

void CameraActor::ActorInput(const struct InputState& states)
{
    float forwardspeed = 0.f;
    float angularSpeed = 0.f;

    if(states.keyboard.GetKetValue(SDL_SCANCODE_W))
    {
        forwardspeed +=300.f;
    }   
    if(states.keyboard.GetKetValue(SDL_SCANCODE_S))
    {
        forwardspeed -=300.f;
    }   
    if(states.keyboard.GetKetValue(SDL_SCANCODE_D))
    {
        angularSpeed +=Math::TwoPi;
    }   
    if(states.keyboard.GetKetValue(SDL_SCANCODE_A))
    {
        angularSpeed -=Math::TwoPi;
    }   
    
    if(states.keyboard.GetKetValue(SDL_SCANCODE_I))
    {
        Vector3 cameraPos = GetPosition();
        SDL_Log("camera pos: %f %f %f ",cameraPos.x,cameraPos.y,cameraPos.z);
        SDL_Log("camera forworad: %f %f %f ",GetForward().x,GetForward().y,GetForward().z);
    };

    m_movecomp->SetForwardSpeed(forwardspeed);
    m_movecomp->SetAngularSpeed(angularSpeed);

}

void CameraActor::SetFootstepSurface(float value)
{
    m_footstep.SetPaused(true);
    m_footstep.SetParameter("Surface",value);
}

