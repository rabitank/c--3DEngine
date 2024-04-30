#include "soundEvent.h"
#include "audioSystem.h"

#include <fmod_studio.hpp>
#include <fmod_errors.h>

SoundEvent::SoundEvent(class AudioSystem* system , unsigned int id)
    :m_system(system),
    m_id(id)
{
}

SoundEvent::SoundEvent()
    :m_system(nullptr),
    m_id(0)
{
}

bool SoundEvent::IsValid()
{
    return (m_system && m_system->GetEventInstance(m_id)) ;
}

void SoundEvent::Resart()
{
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->start();
    }
}

void SoundEvent::Stop(bool allowFadeOut)
{
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        FMOD_STUDIO_STOP_MODE mode = allowFadeOut ?
             FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE; 
        event->stop(mode);
    }

}

void SoundEvent::SetPaused(bool pause)
{
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->setPaused(pause);
    }
    

}

void SoundEvent::SetVolume(float value)
{
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->setVolume(value);
    }

}

void SoundEvent::SetPitch(float value)
{
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->setPitch(value);
    }

}

void SoundEvent::SetParameter(const std::string& name , float value)
{
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->setParameterByName(name.c_str(), value);
    }
}

bool SoundEvent::GetPaused() const
{
    bool retval = false;
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->getPaused(&retval);
    }
    return retval;
    

}

float SoundEvent::GetVolume() const
{
    float retval = 0.f;
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->getVolume(&retval);
    }
    return retval;
}

float SoundEvent::GetPitch() const
{
    float retval = 0.f;
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->getPitch(&retval);
    }
    return retval;
    
}

float SoundEvent::GetParameter(const std::string& name) const
{
    float retval = 0.f;
    auto event = m_system ? m_system->GetEventInstance(m_id) : nullptr;
    if(event)
    {
        event->getParameterByName(name.c_str(), &retval);
    }
    return retval;
    
}

bool SoundEvent::Is3D() const
{
    bool retVal = false;
    auto event = m_system ? m_system->GetEventInstance(m_id):nullptr;
    if(event)
    {
        FMOD::Studio::EventDescription* ed = nullptr;
        event->getDescription(&ed);
        ed->is3D(&retVal);
    }

    return retVal;

}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		// Convert from our coordinates (+x forward, +y right, +z up)
		// to FMOD (+z forward, +x right, +y up)
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

/// @brief set sound transfom in world
/// @param worldTrans 
void SoundEvent::Set3DAttributes(const Matrix4& worldTrans)
{
    bool is3D = false;
    auto event = m_system ? m_system->GetEventInstance(m_id):nullptr;
    if(event)
    {
        FMOD::Studio::EventDescription* ed = nullptr;
        event->getDescription(&ed);
        ed->is3D(&is3D);
        if(is3D)
        {
            FMOD_3D_ATTRIBUTES attributes;
            attributes.position =   VecToFMOD(worldTrans.GetTranslation());
            attributes.forward =    VecToFMOD(worldTrans.GetXAxis());
            attributes.up =         VecToFMOD(worldTrans.GetZAxis());
            /// 速度用于计算多普勒效应
            attributes.velocity = {0.f,0.f,0.f};
            event->set3DAttributes(&attributes);
        }
    }
}




