#pragma once

#include "component.h"

class SpriteComponent: public Component
{
protected:
    /// @brief  ref of true texture
    class Texture* m_texture;
    
    int m_DrawOrder;

    /// @brief describ rect of texture
    int m_texWidth;
    int m_texHeight;

public:
    SpriteComponent(class Actor* owner , int DrawOrder = 100);
    ~SpriteComponent();

    /// @brief should had set rectVAO active 
    /// @param renderer .  
    virtual void Draw(class Shader* shader);
    
    virtual void SetTexture(class Texture* texture);

    int GetDrawOrder() const {return m_DrawOrder;};
    int GetTextureWidth() const { return m_texWidth;};
    int GetTextureHeight() const {return m_texHeight;};

};

