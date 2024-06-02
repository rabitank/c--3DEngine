#include "orbitCamera.h"
#include "actor.h"

OrbitCamera::OrbitCamera(class Actor* owner)
    :CameraComponent(owner),
    m_offset(-400.f,0.f,0.f),
    m_up(0.f,0.f,1.f),
    m_pitchSpeed(0.f),
    m_yawSpeed(0.f)
{
}

void OrbitCamera::Update(float deltatime)
{
    // 2 steps . 1. yaw rot a bit -> then get the right vector
    CameraComponent::Update(deltatime);
    Quaternion yaw(Vector3::UnitZ,m_yawSpeed * deltatime);

    m_offset = Vector3::Transform(m_offset,yaw);
    m_up = Vector3::Transform(m_up,yaw);

    Vector3 forward = -1.f*m_offset;
    forward.Normalize();
    Vector3 right = Vector3::Cross(m_up,forward);
    right.Normalize();

    // 2 steps . 2. pitch rot a bit -> then get the final up vector
    Quaternion pitch(right,m_pitchSpeed*deltatime);
    m_offset = Vector3::Transform(m_offset,pitch);
    m_up = Vector3::Transform(m_up,pitch);

    // create lookat
    Vector3 pos = m_owner->GetPosition()+m_offset;
    const auto viewmat = Matrix4::CreateLookAt(
        pos,
        m_owner->GetPosition(),
        m_up
    );
    SetViewMat(viewmat);
}

