#include "inputComponent.h"
#include "inputSystem.h"

InputComponent::InputComponent(Actor* owner)
: MoveComponent(owner)
{
}

void InputComponent::ProcessInput(const  InputState& states)
{

    float forwardSpeed = 0.f;

    if(states.keyboard.GetKetValue(static_cast<SDL_Scancode>(m_forwardKey)))
    {
        forwardSpeed += m_maxForwardSpeed;
    }
    if(states.keyboard.GetKetValue(static_cast<SDL_Scancode>(m_backKey)))
    {
        forwardSpeed -= m_maxForwardSpeed;
    }
    SetForwardSpeed(forwardSpeed);

    float angluarspeed =  0.f;

    if(states.keyboard.GetKetValue(static_cast<SDL_Scancode>(m_clockwiseKey)))
    {
        angluarspeed -= m_maxRotationSpeed;

    }
    if(states.keyboard.GetKetValue(static_cast<SDL_Scancode>(m_counterClockwiseKey)))
    {
        angluarspeed += m_maxRotationSpeed;
    }
    SetAngularSpeed(angluarspeed);
}
