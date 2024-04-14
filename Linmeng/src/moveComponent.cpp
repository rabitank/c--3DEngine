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
        float rot = m_owner->GetRotation();
        rot+=m_angularSpeed*deltatime;
        m_owner->SetRotation(rot);
    }
    if(!Math::NearZero(m_forwardSpeed))
    {
        Vector2 pos = m_owner->GetPosition();
        pos += m_owner->GetForward() * m_forwardSpeed * deltatime;
        m_owner->SetPosition(pos);
    }    
}
