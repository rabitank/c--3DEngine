#include "asteroid.h"
#include "math.h"
#include "Random.h"
#include "spriteComponent.h"
#include "moveComponent.h"
#include "circleComponent.h"
#include "game.h"
#include "texture.h"
#include "renderer.h"

Asteroid::Asteroid(class Game* ins)
:   Actor(ins)
{
    const Vector2 randpos2 = Random::GetVector(Vector2::Zero,Vector2(1024.f,768.f));
    Vector3 randPos(randpos2.x,randpos2.y,0.f)  ; 
    SetPosition(randPos);

    Quaternion rot = Quaternion( Vector3::UnitZ,Random::GetFloatRange(0.f,Math::TwoPi) );
    SetRotation(rot);

    auto* sc = new SpriteComponent(this); 
    sc->SetTexture( GetGame()->GetRenderer()->GetTexture("Assets/Asteroid.png") );

    auto* mc = new MoveComponent(this);
    mc->SetForwardSpeed(150.f);

    m_circle = new CircleComponent(this);
    m_circle ->SetRadius(40.f);
    
}

Asteroid::~Asteroid()
{
}

