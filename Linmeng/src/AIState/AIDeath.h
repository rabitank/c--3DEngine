#pragma once

#include "AIState.h"

class AIDeath:public AIState
{
public:
    AIDeath(class AIComponent* owner);

    void Update(float deltatime) override;
    void OnExit() override;
    void OnEnter() override;

    const char* GetName() const;
};