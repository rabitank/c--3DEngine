#include "AIPatrol.h"

AIPatrol::AIPatrol(class AIComponent* owner)
: AIState(owner)
{
    
}

void AIPatrol::Update(float deltatime)
{

    
    // TODO: 
    /*
    
    bool dead = IsDead();
    if(dead)
    {
        mOwner->ChangeState("Death");
    }

    */

}

void AIPatrol::OnExit()
{

    
}

void AIPatrol::OnEnter()
{
    
}

const char* AIPatrol::GetName() const
{
    return "Partol";
}
