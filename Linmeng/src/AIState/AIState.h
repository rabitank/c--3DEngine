
#pragma once
#include <string>

/// @brief abstract class
class AIState
{
protected:
    class AIComponent* m_owner;

public:
    AIState(AIComponent* owner)
    : m_owner(owner) {};

    virtual void Update(float deltatime) = 0;
    virtual void OnExit() = 0;
    virtual void OnEnter() = 0;

    /// @brief Getname 直接闭包. use to map
    virtual const char* GetName() const = 0;
};

