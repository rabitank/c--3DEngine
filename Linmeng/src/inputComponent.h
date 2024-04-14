#pragma once

#include "moveComponent.h"


/// @brief Input类进一步绑定速度与按键输入 ,以及做一些相关的规范工作
class InputComponent: public MoveComponent
{

    float m_maxForwardSpeed {250.f};
    float m_maxRotationSpeed{6.f};

    /// @brief linear move
    int m_forwardKey {SDL_SCANCODE_W};
    int m_backKey    {SDL_SCANCODE_S};

    /// @brief angular move 
    int m_clockwiseKey  {SDL_SCANCODE_D};  // 时针方向
    int m_counterClockwiseKey {SDL_SCANCODE_A}; // 逆时针方向


public:
    InputComponent(Actor* owner);

    void ProcessInput(const uint8_t* states) override;


    InputComponent& SetMaxforwardSpeed(int speed) { m_maxForwardSpeed = speed; return *this;};
    InputComponent& SetMaxRotationSpeed(int speed) { m_maxRotationSpeed = speed;return *this;};
    InputComponent& SetForwardKey(int sdlkey) { m_forwardKey = sdlkey;return *this;};
    InputComponent& SetBackkey(int sdlkey) { m_backKey = sdlkey;return *this;};
    InputComponent& SetClockwiseKey(int sdlkey) { m_clockwiseKey = sdlkey;return *this;};
    InputComponent& SetCounterClockwiseKey(int sdlkey) { m_counterClockwiseKey = sdlkey;return *this;};


};

