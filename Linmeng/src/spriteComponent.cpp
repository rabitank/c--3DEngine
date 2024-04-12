#include "spriteComponent.h"

#include "Actor.h"
#include "SDL.h"
#include "math.h"
#include "game.h"

SpriteComponent::SpriteComponent(Actor *owner, int drawOrder)
:Component(owner),m_DrawOrder(drawOrder),m_texWidth(0),m_texHeight(0)
{
    m_owner->GetGame()->AddSprite(this);
}
SpriteComponent::~SpriteComponent(){
    m_owner->GetGame()->RemoveSprite(this);
};

void SpriteComponent::Draw(SDL_Renderer *renderer)
{
    if(m_texture)
    {
        SDL_Rect r ;
        r.w = static_cast<int> (m_texWidth * m_owner->GetScale());
        r.h = static_cast<int> (m_texHeight * m_owner->GetScale());
        r.x = static_cast<int> (m_owner->GetPosition().x + r.w/2);
        r.y = static_cast<int> (m_owner->GetPosition().y + r.h/2);
        
                
        SDL_RenderCopyEx(
            renderer,m_texture,nullptr,&r,
            -math::Todegree(m_owner->GetRotation()),
            nullptr, // defual -> rot center of Texture
            SDL_FLIP_NONE
        );
    }
}

void SpriteComponent::SetTexture(SDL_Texture *texture)
{
    m_texture = texture;
    SDL_QueryTexture(texture,nullptr,nullptr,&m_texWidth,&m_texHeight);
}
