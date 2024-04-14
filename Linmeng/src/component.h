#pragma once
#include "SDL.h"

class Component
{
public:
    /// @brief Ctor
    /// @param updateOrder the lower update earlier . 
    Component(class Actor* Owner , int updateOrder = 100);

    // :m_owner(Owner) , m_updateOrder(updateOrder){}
    // ;

    virtual ~Component();

    virtual void Update(float deltatime);
    virtual void ProcessInput(const uint8_t* states) {};

    int GetUpdateOrder() const { return m_updateOrder;}
    const Actor& GetOwner() const { return *m_owner;}

protected:

    class Actor* m_owner;
    /// @brief  componennt update order in components
    int m_updateOrder;
};



