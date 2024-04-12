#include "actor.h"
#include "component.h"
#include "game.h"
#include <algorithm>

Actor::Actor(Game *game)
:m_state(eActive),m_position({0.f,0.f}),m_scale(1.f),m_rotation(0.f),m_game(game)
{
    m_game->AddActor(this);
}

Actor::~Actor()
{
    m_game->RemoveActor(this);

    ///@attention ~Componennt call RemoveComponents so it can work
    while(m_components.empty())
    {
        delete m_components.back();
    }
}

void Actor::Update(float deltatime)
{
    if( m_state == eActive)
    {
        UpdateComponents(deltatime);
        UpdateActor(deltatime);
    }
}

void Actor::UpdateComponents(float deltatime)
{
    for(auto i: m_components)
    {
        i->Update(deltatime);
    }

}

void Actor::UpdateActor(float deltatime)
{}

Actor &Actor::AddComponents(Component *comp)
{
    m_components.push_back(comp);
    return *this;
}

Actor &Actor::RemonveComponent(Component *comp)
{
    auto i = std::find(m_components.begin(),m_components.end(),comp) ;
    if(i!=m_components.end())
    {
        m_components.erase(i);
    }
    return *this;
}
