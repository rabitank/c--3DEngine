#include "asteroid.h"
#include "math.h"
#include "Random.h"
#include "spriteComponent.h"
#include "moveComponent.h"
#include "circleComponent.h"
#include "game.h"

Asteroid::Asteroid(class Game* ins)
:   Actor(ins)
{
    Vector2 randPos = Random::GetVector(Vector2::Zero,Vector2(1024.f,768.f)); 
    SetPosition(randPos);

    SetRotation(Random::GetFloatRange(0.f,Math::TwoPi));


    auto* sc = new SpriteComponent(this); 
    sc->SetTexture( GetGame()->GetTexture("Assets/Asteroid.png") );

    auto* mc = new MoveComponent(this);
    mc->SetForwardSpeed(150.f);

    m_circle = new CircleComponent(this);
    m_circle ->SetRadius(40.f);

    GetGame()->AddAsteroid(this);


    
}

