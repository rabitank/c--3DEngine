#pragma once

#include "component.h"

class SpriteComponent: public Component
{
protected:
    /// @brief  ref of true texture
    class SDL_Texture* m_texture;
    
    int m_DrawOrder;

    /// @brief describ rect of texture
    int m_texWidth;
    int m_texHeight;

public:
    SpriteComponent(class Actor* owner , int DrawOrder = 100);
    ~SpriteComponent();

    /// @brief Called by Game Ins , rely on Its onwer Actor Data;
    /// @attention its center is Actor;s pos;
    /// @param renderer 
    virtual void Draw(class SDL_Renderer* renderer);

    virtual void SetTexture(SDL_Texture* texture);

    int GetDrawOrder() const {return m_DrawOrder;};
    int GetTextureWidth() const { return m_texWidth;};
    int GetTextureHeight() const {return m_texHeight;};

};

