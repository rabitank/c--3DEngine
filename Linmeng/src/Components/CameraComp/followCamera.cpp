#include "followCamera.h"
#include "actor.h"



FollowCamera::FollowCamera(Actor* owner)
    :CameraComponent(owner),
    m_horzDist(350.f),
    m_vertDist(250.f),
    m_targetDist(100.f),
    m_springConstant(64.f)
{
};

void FollowCamera::Update(float deltatime)
{
    CameraComponent::Update(deltatime);
    // 由弹簧常数估算出的一个阻尼(空气)系数
    float damping = 2.f* Math::Sqrt(m_springConstant);
    
    Vector3 idealPos = ComputeCameraPos();
    // 注意diff的方向
    Vector3 diff = m_actualPos - idealPos;
    // 弹簧加速度总是和 拉伸方向相反(这里简化了质量)
    Vector3 acel = -m_springConstant *diff - damping*m_velocity;
    m_velocity +=acel*deltatime;

    m_actualPos += m_velocity*deltatime;

    Vector3 target = m_owner->GetPosition() +
        m_owner->GetForward()*m_targetDist;
    auto viewmat =  Matrix4::CreateLookAt(m_actualPos,target,Vector3::UnitZ);
    SetViewMat(viewmat);
}

void FollowCamera::SnapToIdeal()
{
    m_actualPos = ComputeCameraPos();
    m_velocity = Vector3::Zero;
    Vector3 target = m_owner->GetPosition() + m_owner->GetForward()*m_targetDist;

    auto viewmat =  Matrix4::CreateLookAt(m_actualPos,target,Vector3::UnitZ);
    SetViewMat(viewmat);
}


Vector3 FollowCamera::ComputeCameraPos() const
{
    Vector3 idelpos = m_owner->GetPosition();
    idelpos -= m_owner->GetForward()*m_horzDist;
    idelpos += Vector3::UnitZ*m_vertDist;
    return idelpos;
}

