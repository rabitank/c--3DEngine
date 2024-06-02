#pragma once
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include "SDL_gamecontroller.h"
#include "Umath.h"

enum ButtonState
{
    ENone,
    EPressed,
    EReleased,
    EHeld
};

class KeyboardState
{
public:
    friend class InputSystem;
     
    bool GetKetValue(SDL_Scancode scancode) const ;
    ButtonState GetKeyState(SDL_Scancode scancode) const;

private:
    /// ptr to sdl's keyboard state
    const uint8_t* m_curState;
    /// @brief local storage states
    uint8_t m_preState[SDL_NUM_SCANCODES];
};
/// @brief MouseState 包含鼠标移动 ,滚轮 与按键. 鼠标移动有两种模式: 相对与位置
class MouseState
{
public:
    friend class InputSystem;

    /// @brief 返回SDL坐标系(左上右下)中的鼠标位置/相对位置
    /// @return 
    const Vector2 GetPosition() const {return m_mousPos;}
    const Vector2 GetScrollWheel() const { return m_scrollWheel;}

    bool IsRelative() const { return m_isRelative;}

    // button
    bool GetButtonValue(int buttonCode) const;
    ButtonState GetButtonState(int buttonCode) const;
private:

    Vector2 m_mousPos;
    Vector2 m_scrollWheel;

    uint32_t m_curButtonsState;
    uint32_t m_preButtonsState;
    // mousepos is in relative mode?
    bool m_isRelative;

};

class ControllerState
{   
public:
	friend class InputSystem;

	// For buttons
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	const Vector2& GetLeftStick() const { return m_leftStick; }
	const Vector2& GetRightStick() const { return m_rightStick; }
	float GetLeftTrigger() const { return   m_leftTrigger; }
	float GetRightTrigger() const { return m_rightTrigger; }

	bool GetIsConnected() const { return m_isConnected; }
private:
	// Current/previous buttons
	Uint8 m_curButtonState[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 m_preButtonState[SDL_CONTROLLER_BUTTON_MAX];
	// Left/right sticks
	Vector2 m_leftStick;
	Vector2 m_rightStick;
	// Left/right trigger
	float m_leftTrigger;
	float m_rightTrigger;
	// Is this controller connected?
	bool m_isConnected;

};


struct InputState
{
    KeyboardState keyboard;
    MouseState mouse;
    ControllerState controller;

};

class InputSystem
{
public:
    InputSystem();

    bool Initialize();
    void Shutdown();

    void PrepareForUpdate();

    void Update();
    
    /// @brief update mouse wheel
    /// @param event 
    void ProcessEvent(union SDL_Event& event);

    const InputState& GetState() const { return m_state;}
    
    void SetRelativeMouseMode(bool value);

private:
    float Filter1D(int input); 
    Vector2 Filter2D(int inputx , int inputy); 
    InputState m_state;

    SDL_GameController* m_controller;
};
