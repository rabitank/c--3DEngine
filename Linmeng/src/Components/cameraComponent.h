#pragma once

#include "component.h"
#include "Umath.h"

class CameraComponent:public Component
{

public:
    CameraComponent(class Actor* owner , int update = 200);

protected:
    void SetViewMat(const Matrix4& view);

};