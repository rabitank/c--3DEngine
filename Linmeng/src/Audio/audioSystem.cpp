#include "audioSystem.h"

#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <SDL_log.h>
#include <vector>

#include "defines.h"
unsigned int AudioSystem::s_nextID = 0;


AudioSystem::AudioSystem(class Game* game)
    :m_game(game),
    m_fmodSys(nullptr),    
    m_coreFmodSys(nullptr)    
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize()
{
    // init fmod debug -> use log version library  
    FMOD::Debug_Initialize(
        FMOD_DEBUG_LEVEL_ERROR,
        FMOD_DEBUG_MODE_TTY
    );

    FMOD_RESULT result;
    result = FMOD::Studio::System::create(&m_fmodSys);
    if(result != FMOD_OK)
    {
        SDL_Log("Failed to create FMOD system: %s",FMOD_ErrorString(result));   
        return false;
    }

    result = m_fmodSys->initialize(
        512,
        FMOD_STUDIO_INIT_NORMAL,
        FMOD_INIT_CHANNEL_LOWPASS,
        nullptr
    );

    if(result != FMOD_OK)
    {
        SDL_Log("Failed to initialize FMOD system: %s",FMOD_ErrorString(result));   
        return false;
    }

    m_fmodSys->getCoreSystem(&m_coreFmodSys);
    ///@attention view : https://www.fmod.com/docs/2.03/studio/getting-events-into-your-game.html about main/Master Bank
    /// And you can only load single bank group
    LoadBank("Assets/Master Bank.strings.bank");
    LoadBank("Assets/Master Bank.bank");

    // LoadBank("Assets/AnMaster.strings.bank");
    // LoadBank("Assets/AnMaster.bank");

    // just init lisstener
    auto viewmat = Matrix4::CreateScale(1.f);
    viewmat *= Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ,0.f));
    SetListener(viewmat);

    return true;
}

void AudioSystem::Shutdown()
{
    UnloadAllBanks();    
    if(m_fmodSys)
    {
        m_fmodSys->release();
    }
}

void AudioSystem::LoadBank(const std::string& name)
{
    #ifdef LM_DEBUG
    SDL_Log("Loading FMOD Bank: %s",name.c_str());
    #endif // DEBUG

    if(m_banks.find(name) != m_banks.end())
    {
        return;
    }

    FMOD::Studio::Bank* bank = nullptr;
    FMOD_RESULT result = m_fmodSys->loadBankFile(
        name.c_str(),
        FMOD_STUDIO_LOAD_BANK_NORMAL,
        &bank
    );

    // event path / name str length;

    const int maxPathLength = 512;
    // bank load success
    if(result == FMOD_OK)
    {
        m_banks.emplace(name,bank);
        bank->loadSampleData();

        int numEvents = 0;
        bank->getEventCount(&numEvents);

        // get bank's events infomation -> map
        if(numEvents > 0 )
        {
            FMOD::Studio::EventDescription** events =  new FMOD::Studio::EventDescription*[numEvents];
            int redvic = 0;
            // load bank eventsDescription ->  local vector
            
            bank->getEventList(events,numEvents,&redvic);
            
            char eventName[maxPathLength];
            // local vector -> eventDesciption map
            for(int i = 0; i<numEvents;i++)
            {
                FMOD::Studio::EventDescription* e = events[i];
                int recived = 0;
                // Get the path of this event (like event:/Explosion2D)
                auto res =  e->getPath(eventName,maxPathLength,&recived);
                #ifdef LM_DEBUG
                SDL_Log("   Loading FMOD Event: %s  %d/%d",eventName,i,numEvents);
                if(!e)
                {
                    SDL_Log("   Failed: %s",FMOD_ErrorString(res) );
                }
                #endif

                m_events.emplace(eventName ,e);
            }
        }
        // get bank's buses infomation -> map
        int numBuses = 0;
        bank->getBusCount(&numBuses);
        if(numBuses>0)
        {
            std::vector<FMOD::Studio::Bus*> buses(numBuses);
            bank->getBusList(buses.data(),numBuses,&numBuses);
            char busesname[maxPathLength];
            for(int i = 0; i<numBuses;i++)
            {
                auto b = buses[i];
                b->getPath(busesname,maxPathLength,nullptr);
                #ifdef LM_DEBUG
                    SDL_Log("   Loading FMOD buses: %s",busesname);
                #endif // DEBUG
                m_buses.emplace(busesname,b);
            }
        }
    }
}


void AudioSystem::UnloadBank(const std::string& name)
{
    auto iter = m_banks.find(name);
    if(iter == m_banks.end())
    {
        return;
    }

    FMOD::Studio::Bank* bank = iter->second;

    int eventsnum = 0;
    bank->getEventCount(&eventsnum);
    const int length = 512;
    if(eventsnum>0)
    {
        std::vector<FMOD::Studio::EventDescription*> events(eventsnum);
        bank->getEventList(events.data(),eventsnum,&eventsnum);
        char eventname[length];
        for(int i = 0;i< eventsnum ; i++)
        {
            FMOD::Studio::EventDescription* e = events[i];
            e->getPath(eventname,length,nullptr);
            auto it = m_events.find(eventname);
            if(it!=m_events.end())
            {
                m_events.erase(it);
            }
        }
    }
    
    int busesnum = 0;
    bank->getBusCount(&busesnum);
    if(busesnum>0)
    {
        std::vector<FMOD::Studio::Bus*> buses(busesnum);
        bank->getBusList(buses.data(),busesnum,&busesnum);
        char busename[length];
        for(int i = 0;i< busesnum ; i++)
        {
            FMOD::Studio::Bus* e = buses[i];
            e->getPath(busename,length,nullptr);
            auto it = m_buses.find(busename);
            if(it!=m_buses.end())
            {
                m_buses.erase(it);
            }
        }
    }
    bank->unloadSampleData();
    bank->unload();

    m_banks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
    for(auto& it : m_banks )
    {
        it.second->unloadSampleData();
        it.second->unload();
    }
    m_banks.clear();
    m_events.clear();
    // but bused still exist? 
}

/// pls check soundevent is valid
SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
    unsigned int retID = 0;
    auto iter = m_events.find(name);

    FMOD_RESULT res;
    if(iter != m_events.end())
    {
        FMOD::Studio::EventInstance* ins = nullptr;
        iter->second->createInstance(&ins);
        if(ins)
        {
            s_nextID++;
            retID = s_nextID;
            m_eventInstance.emplace(retID , ins);
            
            res = ins->start();
            SDL_Log("SoundEvent %s Instance Created ",name.c_str());
            if(res!=FMOD_OK){
                SDL_Log("Event start result:%s", FMOD_ErrorString(res));
            }
        }
        else
        {
            SDL_Log("SoundEvent %s Instance failed! ",name.c_str());
        }
    }

    return SoundEvent(this,retID);
}

void AudioSystem::Update(float deltatime)
{
    std::vector<unsigned int> done;
    for(auto& iter : m_eventInstance)
    {
        FMOD::Studio::EventInstance* e = iter.second;
        FMOD_STUDIO_PLAYBACK_STATE state;
        e->getPlaybackState(&state);
        /// 不播放时释放资源 , 回收至播放过
        if(state == FMOD_STUDIO_PLAYBACK_STOPPED)
        {
            e->release();
            done.emplace_back(iter.first);
        }
    }

    // delete ins in instances map
    for(auto id: done)
    {
        m_eventInstance.erase(id);
    }

    m_fmodSys->update();
    
}

/// Why none name namespace: view https://zhuanlan.zhihu.com/p/646385730#
/// for hide function in source file
namespace
{
    FMOD_VECTOR VecToFMOD(const Vector3& in)
    {
        /// vector our convert (x forward , y right , z up) 
        /// to fmod vec (z forward , x right , y up) 
        FMOD_VECTOR v;
        v.x = in.y;
        v.y = in.z;
        v.z = in.x;
        return v;
    }
}

/// @brief viewMatrix -> camera transform -> set fmod lisenter transform
/// @param viewMatrix 
void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
    auto transform =  viewMatrix;

    transform.Invert();
    FMOD_3D_ATTRIBUTES listener ;
    listener.position = VecToFMOD(transform.GetTranslation()) ;
    listener.forward = VecToFMOD(transform.GetXAxis());
    listener.up = VecToFMOD(transform.GetZAxis());
    listener.velocity = {0.f,0.f,0.f};

    // listener id:0 ?
    m_fmodSys->setListenerAttributes( 0 , &listener );

}

/// @brief failed return 0.f
float AudioSystem::GetBusVolume(const std::string& name) const
{   
    float retVal = 0.f;
    const auto iter = m_buses.find(name);
    if(iter != m_buses.end())
    {
        iter->second->getVolume(&retVal);
    }
    return retVal;

}

/// @brief failed return false
bool AudioSystem::GetBusPaused(const std::string& name) const
{
    bool retVal = false;
    const auto iter = m_buses.find(name);
    if(iter != m_buses.end())
    {
        iter->second->getPaused(&retVal);
    }
    return retVal;

}

void AudioSystem::SetBusVolume(const std::string& name , float volume)
{
    auto iter = m_buses.find(name);
    if(iter != m_buses.end())
    {
        iter->second->setVolume(volume);
    }

}

void AudioSystem::SetBusPaused(const std::string& name,bool pause)
{
    auto iter = m_buses.find(name);
    if(iter != m_buses.end())
    {
        iter->second->setPaused(pause);
    }
}
/// @brief failed return nullptr
FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
    FMOD::Studio::EventInstance* event = nullptr;
    auto iter = m_eventInstance.find(id);
    if(iter != m_eventInstance.end())
    {
        event = iter->second;
    }
    return event;
}



