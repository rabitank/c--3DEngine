#include "debugActor.h"
#include "inputSystem.h"

DebugActor::DebugActor(class Game* game)
    :Actor(game)
{
    
}

void DebugActor::ActorInput(const  InputState& state)
{
    if(state.keyboard.GetKeyState(SDL_SCANCODE_P)
        ==ButtonState::EPressed )
        SDL_Log("debug actor: KeyBoard P pressed " );
    
    if(state.mouse.GetButtonState(SDL_BUTTON_LEFT)
        ==ButtonState::EPressed )
        SDL_Log("debug actor: Mouse leftButton pressed ,\
            Mouse Pos x %f ,Mouse wheel y %f"
            ,state.mouse.GetPosition().x 
            ,state.mouse.GetScrollWheel().y 
        );

    
}
