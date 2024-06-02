/**
 * *****************************************************************************
 * @file        audioSystem.h
 * @brief       使用Fmod作为后台. 所有音频以Event类型进行播放调用     
 * *****************************************************************************
 */
#pragma once


#include <string>
#include <unordered_map>

#include "Umath.h"
#include "soundEvent.h"

/// @brief forward declar of FMOD core & FMOD Studio's concept
namespace FMOD
{
class System;
namespace Studio
{
    class Bank;
    class EventDescription;
    class EventInstance;
    class System;
    class Bus;
}; // namespace Studio

};

class AudioSystem
{

public:
    AudioSystem(class Game* game);
    ~AudioSystem();

    bool Initialize();
    void Shutdown();
    
    void LoadBank(const std::string& name);
    void UnloadBank(const std::string& name);
    void UnloadAllBanks();

    SoundEvent PlayEvent(const std::string& name);

    void Update(float deltatime);

    /// @brief when use positional listens
    void SetListener(const Matrix4& viewMatrix);
    
    float GetBusVolume(const std::string& name) const;
    bool GetBusPaused(const std::string& name) const;
    void SetBusVolume(const std::string& name , float volume);
    void SetBusPaused(const std::string& name,bool pause);

protected:
    friend class SoundEvent;
    FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
    /// @brief set id  of eventIns in order;
    static unsigned int s_nextID;

    class Game* m_game;
    std::unordered_map<std::string , FMOD::Studio::Bank*> m_banks;
    std::unordered_map<unsigned int , FMOD::Studio::EventInstance*> m_eventInstance;
    std::unordered_map<std::string , FMOD::Studio::EventDescription*> m_events;
    std::unordered_map<std::string , FMOD::Studio::Bus*>            m_buses;

    FMOD::Studio::System* m_fmodSys;
    FMOD::System* m_coreFmodSys;


};