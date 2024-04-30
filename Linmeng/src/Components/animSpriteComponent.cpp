#include "animSpriteComponent.h"
#include "texture.h"

void AnimSpriteComponent::Update(float deltatime)
{
    SpriteComponent::Update(deltatime);

    if(!m_animTextures.empty())
    {
        m_curFrame += m_animFps*deltatime;
        
        if(m_loop)
        {
            while(m_curFrame >= m_animTextures.size())
            {
                m_curFrame -= m_animTextures.size();
            }

        }
        else{
            if(m_curFrame >= m_animTextures.size()) m_curFrame = m_animTextures.size()-1;
        }

        SetTexture(m_animTextures[static_cast<int>(m_curFrame)]);
    }

}

void AnimSpriteComponent::SetAnimTextures(const std::vector<Texture *>& textures)
{
    m_animTextures = textures;

}
