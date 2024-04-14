#include "Ship.h"
#include "SDL.h"
#include "game.h"
#include "animSpriteComponent.h"
#include "inputComponent.h"
#include "circleComponent.h"
#include "laser.h"
#include "asteroid.h"

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

    m_circle = new CircleComponent(this);
    m_circle->SetRadius(40.f);
    
}

void Ship::UpdateActor(float deltatiem)
{
    m_laserCoolDown -= deltatiem;
    

    if(GetState()== eActive)
    {
        for(auto i : GetGame()->GetAsteroids())
        {
            if(Intersect(GetCircle() , i->GetCircle() ))
            {
                SetState(ePaused);
                SetPosition(Vector2(1024.f/2,768.f/2));
                m_rebornCoolDown = 2.f;
                
            }
        }
    }
    else if(GetState()==ePaused)
    {
        m_rebornCoolDown -= deltatiem;
        if(m_rebornCoolDown<=0.f)
        {
            SetState(eActive);
            ///@attention this set reborn CoolDown time;
        }
    }

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

