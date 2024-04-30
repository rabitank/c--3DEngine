#include "AIComponent.h"
#include "aiState.h"

void AIComponent::Update(float deltatime)
{
    if(m_currentState)
    {
        m_currentState->Update(deltatime);
    }
}

void AIComponent::ChangeState(const std::string& statename)
{
    if(m_currentState)
        m_currentState->OnExit();
    
    auto it = m_AIStates.find(statename);
    if(it != m_AIStates.end())
    {
        it->second->OnEnter();
        m_currentState = it->second; 
    }
    else{
        SDL_Log("AIComponent ChangedState failed.: no match statename:%s",statename.c_str());
        m_currentState = nullptr;
    }
    
}

void AIComponent::Rigister( AIState* state)
{
    m_AIStates.emplace(state->GetName() , state) ;
}


AIComponent::AIComponent(Actor* owner)
: Component(owner)
{
    
}
