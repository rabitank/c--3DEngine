#include "splineCamera.h"

Vector3 Spline::Compute(size_t startIdx,float t) const
{
    int one = startIdx;
    int two = startIdx + 1;
    int three = startIdx +2 ;

    const Vector3& first = controlPoints[one];
    const Vector3& second = controlPoints[two];
    const Vector3& thrid = controlPoints[three];

    const Vector3& m1 = first + (second - first)*t;
    const Vector3& m2 = thrid + (second - thrid)*t;

    return m1 + (m2-m1)*t;
};

SplineCamera::SplineCamera(class Actor* owner)
    :CameraComponent(owner),
    m_index(0),
    m_t(0.f),
    m_speed(0.5f),
    m_paused(false)

{
}

void SplineCamera::Update(float deltatime)
{
    m_t += deltatime  * m_speed;
    if(m_t > 1.f)
    {
        m_t = 0.f;
        m_index++;
        if(m_index > m_path.GetNumPoints())
    } 

}

void SplineCamera::Restart()
{
    
}
