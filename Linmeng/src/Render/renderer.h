
#pragma once
#include "glad/glad.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "Umath.h"
#include "SDL.h"

struct DirectionalLight
{
    Vector3 m_dirction {0.f,0.f,-1.f};
    Vector3 m_diffuseColor {0.f,0.2f,0.3f};
    Vector3 m_specColor{0.4f,0.5f,0.4f};
};

/**
 * @brief       "Storage" Sprites & Mesh Components & shader && textures && mesh. 
 *              Call Component's Draw & Be Called in Games Update to Render.
 */
class Renderer
{
public:
    Renderer(class Game*);
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight) ;
    void ShutDown();
    void UnloadData();

    void Draw();
    
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(SpriteComponent* sprite);

    void AddMeshComp(class MeshComponent* mesh);
    void RemoveMeshComp(MeshComponent* mesh);

    /// @brief Contained by Renderer
    class Texture* GetTexture(const std::string& filepath);
    /// @brief Contained by Renderer
    class Mesh* GetMesh(const std::string& filepath);

    Vector2 GetWindowSize() const { return Vector2(m_screenWidth,m_screenHeight);}

    Renderer& SetViewMatrix(const Matrix4& viewMat4) {m_viewMat = viewMat4;return *this;};
    Renderer& SetProjMatrix(const Matrix4& projMat4) {m_projMat = projMat4;return *this;};

    void SetAmbientLight(const Vector3& ambient) {m_ambientLight = ambient;}
    DirectionalLight& GetDirectionalLight() {return m_dirctionLight;}


private:
    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader* shader);


    std::vector<SpriteComponent*> m_spriteComps;    
    std::vector<MeshComponent*> m_meshComps; 

    std::unordered_map<std::string , Mesh*> m_meshes; 
    std::unordered_map<std::string , Texture*> m_textures;

    float m_screenWidth     {0}; 
    float m_screenHeight    {0};

    class Game* m_game          {nullptr};
    class Shader* m_spriteShader{nullptr};
    class Shader* m_meshShader  {nullptr};
    class VertexArray* m_spriteVAO {nullptr};

    Matrix4 m_viewMat;
    Matrix4 m_projMat;

    Vector3 m_ambientLight;
    /// @brief 直射光
    DirectionalLight m_dirctionLight;

    SDL_Window* m_window;
    SDL_GLContext m_context;
    

};