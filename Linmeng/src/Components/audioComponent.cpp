#include "audioComponent.h"
#include "actor.h"
#include "audioSystem.h"
#include "game.h"
#include "SDL_log.h"

AudioComponent::AudioComponent(class Actor* owner , int updateOrder)
    :Component(owner,updateOrder)
{
};

AudioComponent::~AudioComponent()
{
    StopAllEvent();   
}

void AudioComponent::Update(float deltatime)
{
    Component::Update(deltatime);

    // clear invalid event; // play is what fmod backend do
    auto iter = m_2Devent.begin();
    while(iter  != m_2Devent.end())
    {
        if(!iter->IsValid())
        {
            // erase and update iter
            iter = m_2Devent.erase(iter);
        }
        else
        {
            ++iter;
        }
        

    }
    iter = m_3Devent.begin();
    while(iter != m_3Devent.end())
    {
        if(!iter->IsValid())
        {
            // erase and update iter
            iter = m_2Devent.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
    
}

void AudioComponent::OnUpdateWorldTransform()
{
    auto transmat =  m_owner->GetWorldTransform();
    for(auto&& i : m_3Devent)
    {
        i.Set3DAttributes(transmat);
    }
}


SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
    SoundEvent e = m_owner->GetGame()->GetAudioSystem()->PlayEvent(name);
    
    if(e.IsValid() )
    {
        if(e.Is3D())
        {
            e.Set3DAttributes(m_owner->GetWorldTransform());
            m_3Devent.push_back(e);
        }
        else 
            m_2Devent.push_back(e);
    }
    else
    {
        SDL_Log("failed find %s Event in AudioSystem" , name.c_str());
    }
    return e ;
}

void AudioComponent::StopAllEvent()
{
    for(auto& e: m_2Devent)
    {
        e.Stop();
    }
    
    for(auto& e: m_3Devent)
    {
        e.Stop();
    }

    m_2Devent.clear();
    m_3Devent.clear();
}
