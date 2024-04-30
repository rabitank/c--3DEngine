#include "meshComponent.h"
#include "actor.h"
#include "game.h"
#include "renderer.h"
#include "shader.h"
#include "vertexArray.h"
#include "texture.h"

MeshComponent::~MeshComponent()
{
    m_owner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(class Shader* shader)
{
    if(m_mesh)
    {
        shader->SetMatrixUniform("u_worldTransform",m_owner->GetWorldTransform());
        shader->SetfloatUniform("u_specPower",m_mesh->GetSpecularPower());
        Texture* t = m_mesh->GetTexture(m_textureIndex);
        if(t)
        {
            t->SetActive();
        }

        auto va =  m_mesh->GetVertexArray();
        va->SetActive();
        glDrawElements(GL_TRIANGLES,va->GetNumOfIndices(),GL_UNSIGNED_INT,nullptr);
        va->SetUnActive();
    }
}

MeshComponent::MeshComponent(class Actor* owner)
    :Component(owner),
    m_mesh(nullptr),
    m_textureIndex(0)
{   

    m_owner->GetGame()->GetRenderer()->AddMeshComp(this);

}
