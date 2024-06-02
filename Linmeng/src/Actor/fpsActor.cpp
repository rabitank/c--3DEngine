#include "fpsActor.h"
#include "CameraComp/FPSCamera.h"
#include "meshComponent.h"
#include "audioComponent.h"
#include "moveComponent.h"
#include "renderer.h"
#include "game.h"
#include "inputSystem.h"

FPSActor::FPSActor(class Game* game)
    :Actor(game),
    m_meshcomp(nullptr),
    m_cameracomp(nullptr),
    m_audiocomp(nullptr),
    m_movecomp(nullptr)
{
    
    game->GetInputSystem()->SetRelativeMouseMode(true);
    m_audiocomp = new AudioComponent(this);
    m_movecomp = new MoveComponent(this);
    
    m_cameracomp = new FPSCamera(this);
    m_cameracomp->SetMaxPitch(Math::Pi/3.f);
    
    
    m_modelA  = new Actor(game);
    m_modelA->SetScale(0.75f);
    m_meshcomp = new MeshComponent(m_modelA);
    m_meshcomp->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));

    m_lastFootstep = 0.f;
    m_stepSound =  m_audiocomp->PlayEvent("event:/Footstep");
    m_stepSound.SetPaused(true);

}

void FPSActor::UpdateActor(float deltatime)
{
    Actor::UpdateActor(deltatime);
    m_lastFootstep -= deltatime;
    // 放完/没放 但是还在走
    if( !Math::NearZero(m_movecomp->GetForwardSpeed()) && m_lastFootstep<= 0.f)
    {
        m_stepSound.SetPaused(false);
        m_stepSound.Resart();
        m_lastFootstep = 0.f;
    }

    const Vector3 modelOffset(Vector3(10.f,10.f,-10.f));
    Vector3 modelPos = GetPosition();
    modelPos += GetForward() * modelOffset.x;
    modelPos += GetRight() * modelOffset.y;
    modelPos += Vector3::UnitZ * modelOffset.z;

    m_modelA->SetPosition(modelPos);
    Quaternion q = GetRotation();
    // 保持模型旋转跟随镜头
    q = Quaternion::Concatenate(q,Quaternion(GetRight(),m_cameracomp->GetPitch()));
    m_modelA->SetRotation(q);
}

void FPSActor::ActorInput(const struct InputState& states)
{
    float forwardspeed = 0.f;
    float StrafSpeed = 0.f;

    if(states.keyboard.GetKetValue(SDL_SCANCODE_W))
    {
        forwardspeed +=400.f;
    }   
    if(states.keyboard.GetKetValue(SDL_SCANCODE_S))
    {
        forwardspeed -=400.f;
    }   
    if(states.keyboard.GetKetValue(SDL_SCANCODE_D))
    {
        StrafSpeed +=400;
    }   
    if(states.keyboard.GetKetValue(SDL_SCANCODE_A))
    {
        StrafSpeed -=400;
    }   
    
    if(states.keyboard.GetKetValue(SDL_SCANCODE_I))
    {
        Vector3 cameraPos = GetPosition();
        SDL_Log("camera pos: %f %f %f ",cameraPos.x,cameraPos.y,cameraPos.z);
        SDL_Log("camera forworad: %f %f %f ",GetForward().x,GetForward().y,GetForward().z);
    };

    m_movecomp->SetForwardSpeed(forwardspeed);
    m_movecomp->SetStrafeSpeed(StrafSpeed);

    int x,y;
    auto pos = states.mouse.GetPosition();
    x = pos.x;
    y = pos.y;

    /// Yaw Update -> depend on moveAngular
    const int maxMouseSpeed = 500;
    const int maxYawSpeed = Math::Pi*8;
    float curYawSpeed = 0.f;
    if(x!=0)
    {
        curYawSpeed = 
            static_cast<float>(Math::Clamp( (x),-maxMouseSpeed,maxMouseSpeed))
            /maxMouseSpeed;
        curYawSpeed*=maxYawSpeed;
    }
    m_movecomp->SetAngularSpeed(curYawSpeed);

    /// pitch Update -> depend on cameraPitch
    const int maxPitchSpeed = Math::Pi*8;
    float curPitchSpeed = 0.f;
    if(y!=0)
    {
        curPitchSpeed= 
            static_cast<float>(Math::Clamp( (y),-maxMouseSpeed,maxMouseSpeed))
            /maxMouseSpeed;
        curPitchSpeed*=maxPitchSpeed;
    }
    m_cameracomp->SetPitchSpeed(curPitchSpeed);
}

void FPSActor::SetFootstepSurface(float value)
{
    m_stepSound.SetPaused(true);
    m_stepSound.SetParameter("Surface",value);
}

void FPSActor::SetVisible(bool isVisiable)
{
 // not camer's meshcomp :(
    m_meshcomp->SetVisible(isVisiable);
}


