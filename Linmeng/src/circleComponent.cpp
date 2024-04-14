#include "circleComponent.h"
#include "actor.h"

CircleComponent::CircleComponent(Actor* onwer)
: Component(onwer)
{

    
}

const Vector2& CircleComponent::GetCenter() const 
{
    return m_owner->GetPosition(); 
}


bool Intersect(const CircleComponent& lv,const CircleComponent& rv)
{
    Vector2 diff = lv.GetCenter() - rv.GetCenter();
    const float lengthSq = diff.LengthSq();
    float radiuSq = lv.GetRaius() + rv.GetRaius();
    radiuSq *= radiuSq;

    return lengthSq <= radiuSq; 
}
