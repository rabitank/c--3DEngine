#pragma once
#include "component.h"
#include <limits>



class MoveComponent:public Component
{
    // rotation . rad/s
    float m_forwardSpeed;
    // forward
    float m_angularSpeed;

public:
    explicit MoveComponent(class Actor* owner );
    void Update(float deltatime ) override;

    float GetForwardSpeed() const {return m_forwardSpeed;}
    float GetAngularSpeed() const {return m_angularSpeed;}

    MoveComponent& SetAngularSpeed(float angularspeed) {m_angularSpeed = angularspeed;return *this; };
    MoveComponent& SetForwardSpeed(float forwardspeed) {m_forwardSpeed = forwardspeed;return *this; };

};



