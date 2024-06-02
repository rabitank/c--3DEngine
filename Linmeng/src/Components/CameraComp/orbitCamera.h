#pragma once

#include "cameraComponent.h"
#include "Umath.h"

class OrbitCamera:public CameraComponent
{

public:
    OrbitCamera(class Actor* owner);

    virtual void Update(float deltatime ) override;

    float GetPitchSpeed() const {return m_pitchSpeed;}
    float GetYawSpeed() const {return m_yawSpeed;}

    void SetPitchSpeed(float pitch) {m_pitchSpeed = pitch; }
    void SetYawSpeed(float yaw) {m_yawSpeed = yaw;}

private:
    Vector3 m_offset;
    Vector3 m_up;

    float m_pitchSpeed;
    float m_yawSpeed;

};