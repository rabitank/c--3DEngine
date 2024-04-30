#pragma once
#include "SDL.h"

class Component
{
public:
    /// @brief Ctor
    /// @param updateOrder the lower update earlier . 
    Component(class Actor* Owner , int updateOrder = 100);

    virtual ~Component();

    virtual void Update(float deltatime);
    virtual void ProcessInput(const uint8_t* states) {};
    /// @brief  an event respond of actor;'s reculculate worldtransform matrix;
    virtual void OnUpdateWorldTransform() {};

    int GetUpdateOrder() const { return m_updateOrder;}


protected:
    const Actor& GetOwner() const { return *m_owner;}
    class Actor* m_owner;

    /// @brief  componennt update order in components
    int m_updateOrder;
};



