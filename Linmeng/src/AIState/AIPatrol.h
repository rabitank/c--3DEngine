#pragma once

#include "AIState.h"

class AIPatrol:public AIState
{
public:
    AIPatrol(class AIComponent* owner);

    void Update(float deltatime) override;
    void OnExit() override;
    void OnEnter() override;

    const char* GetName() const;
};