#pragma once

#include "spriteComponent.h"
#include <vector>
#include "math.h"

class BGSpriteComponent:public SpriteComponent
{
private:

    /// @brief ver texture has a offset, 
    struct BGTexture
    {
        SDL_Texture* texture;
        Vector2 offset;
    };
    /// @brief multi BGTextures , add offset , make them connect each
    std::vector<BGTexture> m_BGTexture;
    Vector2 m_screenSize;
    /// @brief a nagetive value to backforward roll
    float m_scrollSpeed{-50.f};
public:
    BGSpriteComponent( Actor* owner , int updateOrder = 100)
    :SpriteComponent(owner,updateOrder){};
 
    void Update(float deltatime) override;
    void Draw(class SDL_Renderer* renderer) override;

    /// @attention screenSize should be called before SetBGTexture
    /// @param screenSize 
    /// @return 
    BGSpriteComponent& SetScreenSize(Vector2 screenSize) { m_screenSize = screenSize;return *this;};
    BGSpriteComponent& SetBGTexture(const std::vector<SDL_Texture*>& textures);
    BGSpriteComponent& SetScrollSpeed(float speed) { m_scrollSpeed = speed; return *this;};
    
    float GetScrollSpeed() const { return m_scrollSpeed;};

};