#include "actor.h"
#include "component.h"
#include "game.h"
#include "inputSystem.h"
#include <algorithm>

Actor::Actor(Game *game)
:   m_state(eActive),
    m_scale(1.f),
    m_rotation(Quaternion::Identity),
    m_position(Vector3(0.f,0.f,0.f)),
    m_game(game),
    m_recomputeWorldTransform(true)
{
    m_game->AddActor(this);
}

Actor::~Actor()
{
    m_game->RemoveActor(this);

    ///@attention ~Componennt call RemoveComponents so it can work
    while(!m_components.empty())
    {
        delete m_components.back();
    }
}

void Actor::Update(float deltatime)
{
    
    if( m_state == eActive ) 
    {   
        /// try to keep transform fresh
        ComputeWorldTransform();

        UpdateComponents(deltatime);
        UpdateActor(deltatime);

        /// try to keep transform fresh
        ComputeWorldTransform();
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

void Actor::ProcessInput(const InputState& states)
{
    if(m_state == eActive)
    {
        for(auto& c:m_components )
        {
            c->ProcessInput(states);
        }
        ActorInput(states);
    }
    
}

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



void Actor::ComputeWorldTransform()
{
    if(m_recomputeWorldTransform)
    {
        m_recomputeWorldTransform = false;
        m_worldTransform = Matrix4::CreateScale(m_scale);
        m_worldTransform *= Matrix4::CreateFromQuaternion(m_rotation);
        m_worldTransform *= Matrix4::CreateTranslation(m_position);

        for(auto&& ele: m_components)
        {
            ele->OnUpdateWorldTransform();
        }

    }

}

