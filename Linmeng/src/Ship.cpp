#include "Ship.h"
#include "SDL.h"
#include "game.h"
#include "animSpriteComponent.h"
#include "inputComponent.h"
#include "circleComponent.h"
#include "laser.h"

Ship::Ship(Game *game)
:Actor(game) , m_RightSpeed(0.f),m_DownSpeed(0.f)
{
    AnimSpriteComponent* asc = new AnimSpriteComponent(this,110);
    std::vector<SDL_Texture*> textures;

    textures.push_back(game->GetTexture("Assets/Ship.png"));
    
    asc->SetAnimTextures(textures);
    asc->SetLoop(true);
    asc->SetAnimFPS(20.f);
    
    InputComponent* ic = new InputComponent(this);
    
    ic->SetMaxRotationSpeed(10.f).SetMaxforwardSpeed(250.f);

    auto* cc = new CircleComponent(this);
    cc->SetRadius(40.f);
    
}

void Ship::UpdateActor(float deltatiem)
{
    m_laserCoolDown -= deltatiem;


    auto pos = GetPosition();
    pos.x += deltatiem * m_RightSpeed;
    pos.y += deltatiem * m_DownSpeed;

    if(pos.x < 25.f){pos.x = 25.f;}
    else if(pos.x > 1024.f- 25.f){pos.x = 1024.f- 25.f;}
    
    if(pos.y < 25.f){pos.y = 25.f;}
    else if(pos.y > 768.f - 25.f ){pos.y = 768.f-25.f;};
    
    SetPosition(pos);
}

void Ship::ActorProcess(const uint8_t* states)
{
    if(states[SDL_SCANCODE_SPACE] && m_laserCoolDown <=0.f)
    {
        Laser* laser = new Laser(GetGame());   
        laser->SetPosition(GetPosition());
        laser->SetRotation(GetRotation());

        // fresh cooldonw;
        m_laserCoolDown = 0.5f;
    }
}

