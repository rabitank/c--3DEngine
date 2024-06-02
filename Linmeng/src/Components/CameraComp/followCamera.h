#pragma once

#include "cameraComponent.h"

class FollowCamera:public CameraComponent
{
public:
    FollowCamera(class Actor* game);

    void Update(float deltatime) override;
    /// @brief set actual pos to idealPos and clear velocity
    void SnapToIdeal();

    void SetHorzDist(float dist) {m_horzDist = dist;}
    void SetVertDist(float dist) {m_vertDist = dist;}
    void SetTargetDist(float dist) {m_targetDist = dist;}
    void SetSpringConstant(float dist) {m_springConstant = dist;}

private:
    
    Vector3 ComputeCameraPos() const;
    
    Vector3 m_actualPos;
    /// @brief simulate . camera move as a physic entity
    Vector3 m_velocity;

    float m_horzDist;
    float m_vertDist;
    float m_targetDist;

    float m_springConstant;


};