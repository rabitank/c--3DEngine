#include "moveComponent.h"
#include "actor.h"
#include "math.h"

MoveComponent::MoveComponent(class Actor* owner)
:   Component(owner),
    m_angularSpeed(0.f),
    m_forwardSpeed(0.f)
{
}

void MoveComponent::Update(float deltatime)
{

    if(!Math::NearZero(m_angularSpeed))
    {
        auto rot = m_owner->GetRotation();
        rot =  Quaternion::Concatenate(
            rot , 
            Quaternion(Vector3::UnitZ,m_angularSpeed*deltatime) 
        );
        m_owner->SetRotation(rot);
    }

    if(!Math::NearZero(m_forwardSpeed))
    {
        Vector3 pos = m_owner->GetPosition();
        pos += m_owner->GetForward() * m_forwardSpeed * deltatime;
        m_owner->SetPosition(pos);
    }    
}
