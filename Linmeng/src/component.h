#pragma once


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
    
    int GetUpdateOrder() const { return m_updateOrder;}

protected:

    class Actor* m_owner;
    /// @brief  componennt update order in components
    int m_updateOrder;
};



