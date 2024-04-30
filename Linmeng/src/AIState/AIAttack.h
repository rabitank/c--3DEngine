#pragma once

#include "AIState.h"

class AIAttack:public AIState
{
public:
    AIAttack(class AIComponent* owner);

    void Update(float deltatime) override;
    void OnExit() override;
    void OnEnter() override;

    const char* GetName() const;
};