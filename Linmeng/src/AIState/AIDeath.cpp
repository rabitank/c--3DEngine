#include "AIDeath.h"

AIDeath::AIDeath(class AIComponent* owner)
: AIState(owner)
{
}

void AIDeath::Update(float deltatime)
{


}

void AIDeath::OnExit()
{

    
}

void AIDeath::OnEnter()
{
    
}

const char* AIDeath::GetName() const
{
    return "Death";
}
