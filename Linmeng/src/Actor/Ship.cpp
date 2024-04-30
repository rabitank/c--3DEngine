#include "Ship.h"
#include "SDL.h"
#include "game.h"
#include "animSpriteComponent.h"
#include "inputComponent.h"
#include "circleComponent.h"
#include "asteroid.h"
#include "renderer.h"
#include "texture.h"


Ship::Ship(Game *game)
:Actor(game) , m_RightSpeed(0.f),m_DownSpeed(0.f)
{
    AnimSpriteComponent* asc = new AnimSpriteComponent(this,110);
    std::vector<Texture*> textures;

    textures.push_back(game->GetRenderer()->GetTexture("Assets/Ship.png"));
    
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
    //TODO: discard or complite
}



void Ship::ActorInput(const uint8_t* states)
{
    //TODO: discard or complite

}

