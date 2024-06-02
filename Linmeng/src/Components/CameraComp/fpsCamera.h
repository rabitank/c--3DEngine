#pragma once

#include "cameraComponent.h"
#include "Umath.h"

class FPSCamera:public CameraComponent
{

public:
    FPSCamera(class Actor* owner);
    
    virtual void Update(float deltatime ) override;

    float GetPitch() const {return m_pitch;}
    float GetPitchSpeed() const {return m_pitchSpeed;}
    float GetMaxPitch() const {return m_maxPitch;}

    void SetPitchSpeed(float pitchSpeed) { m_pitchSpeed = pitchSpeed;  }
    void SetMaxPitch(float maxPitch) { m_maxPitch = maxPitch;  }

private:
    float m_pitchSpeed;
    float m_maxPitch;
    float m_pitch;


};