#include "laser.h"
#include "game.h"
#include "moveComponent.h"
#include "spriteComponent.h"
#include "circleComponent.h"
#include "asteroid.h"

void Laser::UpdateActor(float deltatime)
{
    m_deathTimer -= deltatime;
    if(m_deathTimer <= 0.f)
    {
        SetState(eDead);
    }
    else{
        for(auto ast: GetGame()->GetAsteroids())
        {
            if(Intersect(*m_circle,ast->GetCircle()))
            {
                SetState(eDead);
                ast->SetState(eDead);
                break;
            }
        }
    }
}

Laser::Laser(Game* ins)
: Actor(ins)
{
    auto* mc = new MoveComponent(this);
    mc->SetForwardSpeed(800.f);

    auto* sc = new SpriteComponent(this);
    sc->SetTexture(GetGame()->GetTexture("Assets/Laser.png"));
    
    m_circle = new CircleComponent(this);
    m_circle->SetRadius(11.f);

}
