#include "BGSpriteComponent.h"
#include "renderer.h"
#include "shader.h"
#include "actor.h"
#include "texture.h"
#include "game.h"
#include "renderer.h"

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

void BGSpriteComponent::Draw(Shader *shader)
{
    auto it = m_BGTexture.begin();
    
    for(;it!= m_BGTexture.end();it++)
    {
        auto texture = *it;
        Matrix4 scaleMat = Matrix4::CreateScale(
            static_cast<float>(m_texWidth),
            static_cast<float>(m_texHeight),
            1.f
        );
        auto wordMat = scaleMat * m_owner->GetWorldTransform();
        Matrix4 translate = Matrix4::CreateTranslation(
            Vector3(texture.offset.x,texture.offset.y,0.f)
        );

        texture.texture->SetActive();
        shader->SetMatrixUniform("u_worldTransform",translate);
        shader->SetActive();
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);    
    };
}

BGSpriteComponent &BGSpriteComponent::SetBGTexture(const std::vector<Texture *> &textures)
{
    m_BGTexture.clear();
    auto size = m_owner->GetGame()->GetRenderer()->GetWindowSize();
    int count = 0;
    for(auto i : textures)
    {
        //TODO: test coord.
        m_BGTexture.push_back({i , Vector2{count * size.x , size.y}});
        count++;
    }
    return *this;
}