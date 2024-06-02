#include "inputSystem.h"
#include <cstring>
#include "SDL.h"

bool KeyboardState::GetKetValue(SDL_Scancode scancode) const
{
    return m_curState[scancode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode scancode) const
{
    if(m_preState[scancode] != 1)
    {
        if(m_curState[scancode] != 1)
        {
            return ButtonState::ENone;
        }
        else
        {
            return ButtonState::EPressed;
        }
    }
    else
    {
        if(m_curState[scancode] != 1)
        {
            return ButtonState::EReleased;
        }
        else
        {
            return ButtonState::EHeld;
        }
    }
    

}

bool MouseState::GetButtonValue(int buttonCode) const
{
    return (SDL_BUTTON(buttonCode) & m_curButtonsState ) == 1;
}

ButtonState MouseState::GetButtonState(int buttonCode) const
{
    if( (SDL_BUTTON(buttonCode) & m_preButtonsState) != 1)
    {
        if((SDL_BUTTON(buttonCode) & m_curButtonsState)!= 1)
        {
            return ButtonState::ENone;
        }
        else
        {
            return ButtonState::EPressed;
        }
    }
    else
    {
        if((SDL_BUTTON(buttonCode) & m_curButtonsState)!= 1)
        {
            return ButtonState::EReleased;
        }
        else
        {
            return ButtonState::EHeld;
        }
    }
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
    return m_curButtonState[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
    if (m_preButtonState[button] == 0)
	{
		if (m_curButtonState[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (m_curButtonState[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

InputSystem::InputSystem()
{
    
}

bool InputSystem::Initialize()
{
    m_state.keyboard.m_curState = SDL_GetKeyboardState(nullptr);
    memset(m_state.keyboard.m_preState,0,SDL_NUM_SCANCODES);
    

    m_state.mouse.m_curButtonsState = 0;
    m_state.mouse.m_preButtonsState = 0;

    m_controller = SDL_GameControllerOpen(0);
	// Initialize controller state
	m_state.controller.m_isConnected = (m_controller != nullptr);
	memset(m_state.controller.m_curButtonState, 0,
		SDL_CONTROLLER_BUTTON_MAX);
	memset(m_state.controller.m_preButtonState, 0,
		SDL_CONTROLLER_BUTTON_MAX);
    return true;
};

void InputSystem::Shutdown()
{
    
}

/// @brief in one frame. call before SDL_PollEvent(update sdl's keyboard state)
void InputSystem::PrepareForUpdate()
{
    memcpy(m_state.keyboard.m_preState,m_state.keyboard.m_curState,SDL_NUM_SCANCODES);

    memcpy(m_state.keyboard.m_preState,m_state.keyboard.m_curState,SDL_NUM_SCANCODES);

    m_state.mouse.m_preButtonsState = m_state.mouse.m_curButtonsState;
    /// @attention TODO: why an error?
    // m_state.mouse.m_isRelative = false;
    m_state.mouse.m_scrollWheel = Vector2::Zero;

    memcpy(m_state.controller.m_preButtonState, m_state.controller.m_curButtonState,
        SDL_CONTROLLER_BUTTON_MAX);
    
}

void InputSystem::Update()
{
    // mouse
    int x = 0 , y = 0;
    if(m_state.mouse.m_isRelative)
    {
        m_state.mouse.m_curButtonsState = 
            SDL_GetRelativeMouseState(&x,&y);
    }
    else
    {
        m_state.mouse.m_curButtonsState =
            SDL_GetMouseState(&x,&y); 
    }

    m_state.mouse.m_mousPos.x = static_cast<float>(x);
    m_state.mouse.m_mousPos.y = static_cast<float>(y);

    //Contorller button
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		m_state.controller.m_curButtonState[i] =
			SDL_GameControllerGetButton(m_controller, 
				SDL_GameControllerButton(i));
	}

	// Triggers
	m_state.controller.m_leftTrigger =
		Filter1D(SDL_GameControllerGetAxis(m_controller,
			SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	m_state.controller.m_rightTrigger =
		Filter1D(SDL_GameControllerGetAxis(m_controller,
			SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

	// Sticks
	x = SDL_GameControllerGetAxis(m_controller,
		SDL_CONTROLLER_AXIS_LEFTX);
	y = -SDL_GameControllerGetAxis(m_controller,
		SDL_CONTROLLER_AXIS_LEFTY);
	m_state.controller.m_leftStick = Filter2D(x, y);

	x = SDL_GameControllerGetAxis(m_controller,
		SDL_CONTROLLER_AXIS_RIGHTX);
	y = -SDL_GameControllerGetAxis(m_controller,
		SDL_CONTROLLER_AXIS_RIGHTY);
	m_state.controller.m_rightStick = Filter2D(x, y);

}

void InputSystem::ProcessEvent(SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_MOUSEWHEEL:
            m_state.mouse.m_scrollWheel = Vector2(
                static_cast<float>(event.wheel.x),
                static_cast<float>(event.wheel.y)
            );
        break;
    default: 
        break;
    }
}

void InputSystem::SetRelativeMouseMode(bool value)
{
    SDL_bool sb = value ? SDL_TRUE : SDL_FALSE;
    SDL_SetRelativeMouseMode(sb);
    m_state.mouse.m_isRelative = value;

}

float InputSystem::Filter1D(int input)
{
    const int deadZone = 250;
    const int maxValue = 30000;

	float retVal = 0.0f;

	// Take absolute value of input
	int absValue = input > 0 ? input : -input;
	// Ignore input within dead zone
	if (absValue > deadZone)
	{
		// Compute fractional value between dead zone and max value
		retVal = static_cast<float>(absValue - deadZone) /
		(maxValue - deadZone);
		// Make sure sign matches original value
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// Clamp between -1.0f and 1.0f
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputx , int inputy)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// Make into 2D vector
	Vector2 dir;
	dir.x = static_cast<float>(inputx);
	dir.y = static_cast<float>(inputy);

	float length = dir.Length();

	// If length < deadZone, should be no input
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// Calculate fractional value between
		// dead zone and max value circles
		float f = (length - deadZone) / (maxValue - deadZone);
		// Clamp f between 0.0f and 1.0f
		f = Math::Clamp(f, 0.0f, 1.0f);
		// Normalize the vector, and then scale it to the
		// fractional value
		dir *= f / length;
	}

	return dir;  
}



