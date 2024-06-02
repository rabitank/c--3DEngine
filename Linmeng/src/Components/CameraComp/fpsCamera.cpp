#include "FPSCamera.h"
#include "renderer.h"
#include "actor.h"
#include "game.h"

FPSCamera::FPSCamera(Actor* owner)
    :CameraComponent(owner),
    m_pitch(0.f),
    m_pitchSpeed(0.f),
    m_maxPitch(Math::Pi/2.f)
{
}

void FPSCamera::Update(float deltatime)
{
    CameraComponent::Update(deltatime);

    Vector3 cameraPos = m_owner->GetPosition();

    m_pitch += deltatime*m_pitchSpeed;
    m_pitch = Math::Clamp(m_pitch,-m_maxPitch,m_maxPitch);
    Quaternion rot = Quaternion(m_owner->GetRight(),m_pitch);
    Vector3 viewForward = Vector3::Transform(m_owner->GetForward(),rot);

    Vector3 target = cameraPos + viewForward * 100.f;
    Vector3 up = Vector3::Transform(Vector3::UnitZ,rot);

    auto viewmat = Matrix4::CreateLookAt(cameraPos,target,up);

    SetViewMat(viewmat);
}
