#include "component.h"
#include "actor.h"


Component::Component(Actor * Owner, int updateOrder)
:m_owner(Owner),m_updateOrder(updateOrder)
{
    m_owner->AddComponents(this);
}

Component::~Component()
{
    m_owner->RemonveComponent(this);
}

void Component::Update(float deltatime)
{
}
