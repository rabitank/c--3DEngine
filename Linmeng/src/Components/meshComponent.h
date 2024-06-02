#pragma once
#include "component.h"
#include "mesh.h"

/**
 * @brief   aMeshComp 
 *          Ref a Mesh. Be could in renderer Draw Mesh
 * 
 */
class MeshComponent:public Component
{
public:
    MeshComponent(class Actor* owner);
    ~MeshComponent();
    
    /// @brief accept a shader draw its mesh( VAO , textures ...)
    /// @param shader 
    virtual void Draw(class Shader* shader);

    /// @brief basic set m_mesh member. 
    /// @param mesh 
    virtual void SetMesh(class Mesh* mesh) { m_mesh = mesh; };

    void SetVisible(bool visible){m_visible = visible;};
    bool GetVisible() const {return m_visible;};


protected:
    Mesh* m_mesh;
    size_t m_textureIndex;
    bool m_visible; 
};  