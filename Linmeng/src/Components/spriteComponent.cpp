#include "spriteComponent.h"

#include "Actor.h"
#include "SDL.h"
#include "math.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "shader.h"

SpriteComponent::SpriteComponent(Actor *owner, int drawOrder)
:Component(owner),m_DrawOrder(drawOrder),m_texWidth(0),m_texHeight(0)
{
    m_owner->GetGame()->GetRenderer()->AddSprite(this);
}
SpriteComponent::~SpriteComponent(){
    m_owner->GetGame()->GetRenderer()->RemoveSprite(this);
};

void SpriteComponent::Draw(class Shader* shader)
{
    if(m_texture)
    {
        /// VAO is normalized in [0,1], must scale agin
        Matrix4 scaleMat = Matrix4::CreateScale(
            static_cast<float>(m_texWidth),
            static_cast<float>(m_texHeight),
            1.f
        );
        auto worldMat = scaleMat * m_owner->GetWorldTransform();

        m_texture->SetActive();
        shader->SetMatrixUniform("u_worldTransform",worldMat);
        //  使用已启用数组中的计数顺序元素 view: https://blog.csdn.net/aoxuestudy/article/details/122539069
        /// GL_UNSIGNED_INT 指的 Indeicesbuffer类型
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);    
    }
}


void SpriteComponent::SetTexture(Texture *texture)
{
    m_texture = texture;
    m_texWidth = texture->GetWidth();
    m_texHeight = texture->GetHeight();

}
