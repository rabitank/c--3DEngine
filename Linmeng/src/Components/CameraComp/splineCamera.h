#pragma once

#include "cameraComponent.h"
#include "Umath.h"

#include <vector>

struct Spline
{
    // control point of a line. 
    // a point need two point around to draw line.so n+2.
    std::vector<Vector3> controlPoints;
    Vector3 Compute(size_t startIdx,float t) const;
    size_t GetNumPoints() const {return controlPoints.size();};
};

/// @brief  a camer comp move with spline;
class SplineCamera:public CameraComponent
{

public:
    SplineCamera(class Actor* owner);

    virtual void Update(float deltatime ) override;

    void  Restart();

    void SetSpline(const Spline& spline) { m_path = spline;}
    void SetSpeed(float speed) {m_speed = speed; }
    void SetPaused(bool pause) { m_paused = pause;}

private:
    Spline m_path;
    size_t m_index;
    float m_t;

    float m_speed;
    float m_paused;

};