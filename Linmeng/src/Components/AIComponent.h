
#pragma once

#include "component.h"
#include "pch.h"

class AIComponent:public Component
{
    std::unordered_map<std::string , class AIState*> m_AIStates;
    AIState* m_currentState;

public:
    AIComponent(Actor* owner);
    void Update(float deltatime) override;
    void ChangeState(const std::string& statename);
    void Rigister(AIState* state);

};