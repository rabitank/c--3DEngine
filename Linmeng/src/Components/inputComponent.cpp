#include "inputComponent.h"

InputComponent::InputComponent(Actor* owner)
: MoveComponent(owner)
{
}

void InputComponent::ProcessInput(const uint8_t* states)
{

    float forwardSpeed = 0.f;

    if(states[m_forwardKey])
    {
        forwardSpeed += m_maxForwardSpeed;
    }
    if(states[m_backKey])
    {
        forwardSpeed -= m_maxForwardSpeed;
    }
    SetForwardSpeed(forwardSpeed);

    float angluarspeed =  0.f;

    if(states[m_clockwiseKey])
    {
        angluarspeed -= m_maxRotationSpeed;

    }
    if(states[m_counterClockwiseKey])
    {
        angluarspeed += m_maxRotationSpeed;
    }
    SetAngularSpeed(angluarspeed);
}


