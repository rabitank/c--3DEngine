#include "BGSpriteComponent.h"
#include "SDL.h"
#include "actor.h"

void BGSpriteComponent::Update(float deltatime)
{
    SpriteComponent::Update(deltatime);
    for(auto& i: m_BGTexture)
    {
        i.offset.x+=m_scrollSpeed *deltatime;
        
        // 移出画面时
        if(i.offset.x < -m_screenSize.x) 
        {
            // 将该图移到队列右边
            i.offset.x += ((m_BGTexture.size() ) * m_screenSize.x) -1; 
        }
    }

}

void BGSpriteComponent::Draw(SDL_Renderer *renderer)
{
    auto it = m_BGTexture.begin();
    for(;it!= m_BGTexture.end();it++)
    {
        SDL_Rect r;
        r.w = static_cast<int>(m_screenSize.x);
        r.h = static_cast<int>(m_screenSize.y);
        r.x = static_cast<int>(m_owner->GetPosition().x - r.w/2 + it->offset.x );
        r.y = static_cast<int>(m_owner->GetPosition().y - r.h/2 + it->offset.y);
        SDL_RenderCopy(renderer,it->texture,nullptr,&r);
    };
}

BGSpriteComponent &BGSpriteComponent::SetBGTexture(const std::vector<SDL_Texture *> &textures)
{
    m_BGTexture.clear();

    int count = 0;
    for(auto i : textures)
    {
        m_BGTexture.push_back({i , Vector2{count * m_screenSize.x , 0.f}});
        count++;
    }
    return *this;
}