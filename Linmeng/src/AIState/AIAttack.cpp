#include "AIAttack.h"

AIAttack::AIAttack(class AIComponent* owner)
: AIState(owner)
{
}

void AIAttack::Update(float deltatime)
{


}

void AIAttack::OnExit()
{

    
}

void AIAttack::OnEnter()
{
    
}

const char* AIAttack::GetName() const
{
    return "Attack";
}
