#include "Ship.h"
#include "SDL.h"
#include "game.h"
#include "animSpriteComponent.h"

Ship::Ship(Game *game)
:Actor(game) , m_RightSpeed(0.f),m_DownSpeed(0.f)
{
    AnimSpriteComponent* asc = new AnimSpriteComponent(this,110);
    std::vector<SDL_Texture*> textures;

    textures.push_back(game->GetTexture("Assets/Ship01.png"));
    textures.push_back(game->GetTexture("Assets/Ship02.png"));
    textures.push_back(game->GetTexture("Assets/Ship03.png"));
    textures.push_back(game->GetTexture("Assets/Ship04.png"));
    
    asc->SetAnimTextures(textures);
    asc->SetLoop(true);
    asc->SetAnimFPS(20.f);

}

void Ship::UpdateActor(float deltatiem)
{
    auto pos = GetPosition();

    pos.x += deltatiem * m_RightSpeed;
    pos.y += deltatiem * m_DownSpeed;

    if(pos.x < 25.f){pos.x = 25.f;}
    else if(pos.x > 500.f){pos.x = 500.f;}
    
    if(pos.y < 25.f){pos.y = 25.f;}
    else if(pos.y > 1024.f - 25.f ){pos.y = 1024.f-25.f;};
    
    SetPosition(pos);
}

void Ship::ProccessKeyboard(const uint8_t *state)
{
    m_RightSpeed = 0.f;
    m_DownSpeed = 0.f;

    // use += -= can dispatch the case that two Key were pressed 
    if(state[SDL_SCANCODE_D])
    {
        m_RightSpeed += 250.f;  
    }
    if(state[SDL_SCANCODE_A])
    {
        m_RightSpeed -= 250.f;  
    }

    if(state[SDL_SCANCODE_W])
    {
        m_DownSpeed -= 300.f;  
    }
    if(state[SDL_SCANCODE_S])
    {
        m_DownSpeed += 300.f;  
    }

}
