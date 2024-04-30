#pragma once
#include <string>
#include <vector>


/**
 * @brief An Mesh 
 *        contains itself VAO(VB , IB) , SHaderName , SpecPower
 *        ref  VAOFormat , Texture .
 *    
 *        currently . we define an json file to describe a mesh
 */
class Mesh
{

public:
    Mesh();
    ~Mesh();

    /// @brief Load a gpmesh file to mesh
    /// @param filepath accept a .gpmesh (json) file 
    /// @param renderer provied Texture ref 
    /// @return is successed?
    bool Load(const std::string& filepath , class Renderer* renderer);
    /// @brief delete meshes' VAO
    void Unload();

    class VertexArray* GetVertexArray() const {return m_vertexArray;};
    class Texture* GetTexture(size_t index);

    const std::string& GetShaderName()   const { return m_shadername;};
    const std::string& GetVertexFormat() const { return m_vertexFormat;};

    float GetSpecularPower() const {return m_specPower;};
private:
    std::vector<Texture*> m_textures;
    VertexArray* m_vertexArray;
    std::string m_shadername;
    std::string m_vertexFormat;
    float m_radius;     // 球包围盒半径
    float m_specPower;  //镜面反射强度

};