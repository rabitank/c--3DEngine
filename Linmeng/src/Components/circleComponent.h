#pragma once

#include "component.h"
#include "Umath.h"

class CircleComponent:public Component
{

    float m_radius{10.f};
    
public:
    CircleComponent(Actor* onwer );

    CircleComponent& SetRadius(float radius) {m_radius = radius; return *this;}
    float GetRaius() const {return m_radius;}
    
    Vector3 GetCenter() const ;

};

bool Intersect(const CircleComponent& lv,const CircleComponent& rv );

