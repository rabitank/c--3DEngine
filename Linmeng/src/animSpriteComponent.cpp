#include "animSpriteComponent.h"




void AnimSpriteComponent::Update(float deltatime)
{
    SpriteComponent::Update(deltatime);
    if(m_animTextures.size()>0)
    {
        m_curFrame += m_animFps*deltatime;
        
        while(m_curFrame > m_animTextures.size())
        {
            m_curFrame -= m_animTextures.size();
        }

        SetTexture(m_animTextures[static_cast<int>( m_curFrame)]);
    }

}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture *>& textures)
{
    m_animTextures = textures;

}
