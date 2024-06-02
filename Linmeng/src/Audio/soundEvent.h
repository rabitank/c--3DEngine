#pragma once
#include <string>
#include "Umath.h"

/// @brief should pachaged in AudioSystem. construct SoundEvent with SoundSysten
class SoundEvent
{
public:
    SoundEvent();
    bool IsValid();

    /// @brief to begning 
    void Resart();
    void Stop(bool allowFadeOut = true);

    void SetPaused(bool pause);
    void SetVolume(float value);
    void SetPitch(float value);
    void SetParameter(const std::string& name , float value);

    bool  GetPaused()       const;
    float GetVolume()       const;
    float GetPitch()        const;
    float GetParameter(const std::string& name)    const;

    /// @brief 3D pos effective
    bool Is3D() const;
    void Set3DAttributes(const Matrix4& worldTrans);

protected:
    friend class AudioSystem;
    /// @brief only SoundSysten can build it
    SoundEvent(class AudioSystem* system , unsigned int id);
private:
    class AudioSystem* m_system;
    unsigned int m_id;

};