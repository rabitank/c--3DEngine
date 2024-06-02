#include "orbitActor.h"
#include "CameraComp/orbitCamera.h"
#include "meshComponent.h"
#include "moveComponent.h"
#include "game.h"
#include "renderer.h"
#include "inputSystem.h"

OrbitActor::OrbitActor(class Game* game)
    :Actor(game),
    m_meshcomp(nullptr),
    m_cameracomp(nullptr)
{
    game->GetInputSystem()->SetRelativeMouseMode(true);

    m_meshcomp  =  new MeshComponent(this);
    m_meshcomp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
    SetPosition(Vector3(0.f,0.f,-100.f));
    
    m_cameracomp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const struct InputState& state)
{

    const auto relapos = state.mouse.GetPosition();
    if(state.mouse.GetButtonValue(SDL_BUTTON_LEFT))
    {
        const int maxMouseSpeed = 500;
        const float maxorbitSpeed = Math::Pi *8;
        float yawspeed = 0.f;
        if(relapos.x!=0)
        {
            yawspeed = static_cast<float>(relapos.x)/maxMouseSpeed;
            yawspeed*= maxorbitSpeed;
        }
        m_cameracomp->SetYawSpeed(yawspeed);

        float pitchSpeed = 0.f;
        if(relapos.y!=0)
        {
            pitchSpeed = static_cast<float>(relapos.y)/maxMouseSpeed;
            pitchSpeed *= maxorbitSpeed;
        }
        m_cameracomp->SetPitchSpeed(pitchSpeed);
    }
}

void OrbitActor::SetVisible(bool visible)
{
    m_meshcomp->SetVisible(visible);
}



