#include "renderer.h"
#include "2DComp/spriteComponent.h"
#include "meshComponent.h"
#include "texture.h"
#include "game.h"
#include "vertexArray.h"
#include "shader.h"
#include "mesh.h"

#include "SDL.h"
#include "glad/glad.h"
#include <algorithm>

Renderer::Renderer(Game* game)
    :m_game(game),
    m_spriteShader(nullptr),
    m_meshShader(nullptr)
{

}

Renderer::~Renderer()
{
    
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION , 4);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION , 6);
    // 不允许废弃函数
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
    // 设置图像位宽
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // 启用双缓冲
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
    // 启用加速
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
    
    m_window  = SDL_CreateWindow("Linmeng" ,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_screenWidth,
        m_screenHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
    if(!m_window)
    {
        SDL_Log("main window %s created failed!","Linmemg");
        return false;
    }
    m_context =  SDL_GL_CreateContext(m_window);
    
    SDL_GL_MakeCurrent(m_window, m_context);

    // 导入gl的函数实现

    if( gladLoadGL() != GL_TRUE )
    {
        SDL_Log("Renderer: glad init failed!");
        return false;
    };
    // vsync open
    if(SDL_GL_SetSwapInterval(-1) != -1)
    {
        SDL_Log("open Adaptive vsync success");
    }
    else{
        if(SDL_GL_SetSwapInterval(1) != -1)
        {
            SDL_Log("open vsync success");
        }
        else
        {
            SDL_GL_SetSwapInterval(0); //vsync close
		    SDL_Log("close vsync");
        }
    }
    
    // clear gl error for load shader;
    glGetError();
    if(!LoadShaders())
    {
        SDL_Log("Renderer: Load Shader failed!");
        return false;
    }
    // create quad vao for SpriteRender
    CreateSpriteVerts();
    
    return true;
}

bool Renderer::LoadShaders()
{
    auto curshader = new Shader();
    if(curshader->Load("Shaders/sprite.vert","Shaders/sprite.frag") )
    {
        m_spriteShader = curshader;
    }
    else
    {
        SDL_Log("Renderer: Load Sprite Shader failed!");
        return false;
    }
    
    m_spriteShader->SetActive();
    auto viewProj = Matrix4::CreateSimpleViewProj(m_screenWidth,m_screenHeight);
    m_spriteShader->SetMatrixUniform("u_viewProj", viewProj);
    
    curshader = new Shader();
    if(curshader->Load("Shaders/phong.vert","Shaders/phong.frag") )
    {
        m_meshShader = curshader;
    }
    else 
    {
        SDL_Log("Renderer: Load mesh Shader failed!");
        return false;
    };
    m_meshShader->SetActive();
    //view: https://zhuanlan.zhihu.com/p/66384929 https://zhuanlan.zhihu.com/p/66240124 关于如何理解lookat,以及它和摄像机逆矩阵的关系(等同)
    m_viewMat = Matrix4::CreateLookAt(Vector3::Zero , Vector3::UnitX , Vector3::UnitZ );
    
    m_projMat = Matrix4::CreatePerspectiveFOV(
        Math::ToRadians(90.f) ,
        m_screenWidth,m_screenHeight,
        10.f,10000.f
    );

    m_meshShader->SetMatrixUniform("u_viewProj", m_viewMat * m_projMat);

    return true;   
}

void Renderer::CreateSpriteVerts()
{
    std::vector<float> vertsbuffer{
        -0.5f,-0.5f,0.f, 0.f,0.f,0.f, 0.f,0.f,
        -0.5f, 0.5f,0.f, 0.f,0.f,0.f, 0.f,1.f,
         0.5f, 0.5f,0.f, 0.f,0.f,0.f, 1.f,1.f,
         0.5f,-0.5f,0.f, 0.f,0.f,0.f, 1.f,0.f,
    };
    std::vector<unsigned int> indecbuffer{
        0,1,2,2,3,0
    };
    m_spriteVAO  = new VertexArray(vertsbuffer.data(),4,indecbuffer.data(),indecbuffer.size());
}


void Renderer::ShutDown()
{
    delete m_spriteVAO;

    m_spriteShader->Unload();
    delete m_spriteShader;
    
    m_meshShader->Unload();
    delete m_meshShader;
    
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
}

void Renderer::UnloadData()
{
    for(auto&& i: m_textures)
    {
        i.second->Unload();
        delete i.second;
    }
    m_textures.clear();

    for(auto&& i: m_meshes)
    {
        i.second->Unload();
        delete i.second;
    }
    m_meshes.clear();

}

void Renderer::Draw()
{
    glClearColor(0.1f,0.1f,0.1f,1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 清除 颜色缓冲, 深度缓冲 
    // 开启深度测试 关闭混合
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    m_meshShader->SetActive();
    m_meshShader->SetMatrixUniform("u_viewProj",m_viewMat * m_projMat);
    SetLightUniforms(m_meshShader);

    for(auto&& ele:m_meshComps )
    {
        if(ele->GetVisible())
            ele->Draw(m_meshShader);
    };

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);
    m_spriteShader->SetActive();
    m_spriteVAO->SetActive();
    /// 通常为绘制UI
    for(auto&& ele:m_spriteComps)
    {
        ele->Draw(m_spriteShader);
    }
    

    SDL_GL_SwapWindow(m_window); // swap buffer -> to shower;
    
}

void Renderer::AddSprite( SpriteComponent* sprite)
{
    int order = sprite->GetDrawOrder();
    auto it = m_spriteComps.begin();
    for(;it!=m_spriteComps.end();it++)
    {
        if(order<=(*it)->GetDrawOrder())
        {
            break;
        }
    }
    m_spriteComps.insert(it,sprite);
    

}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
    auto it = std::find(m_spriteComps.begin(),m_spriteComps.end(),sprite);
    /// cant swap & pop. we need remain the order 
    m_spriteComps.erase(it);
}

void Renderer::AddMeshComp(class MeshComponent* mesh)
{
    m_meshComps.push_back(mesh); // 3D并不使用画家算法
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
    auto it = std::find(m_meshComps.begin(),m_meshComps.end(),mesh);
    if(it != m_meshComps.end())
    {
        m_meshComps.erase(it);
    }
}


Texture* Renderer::GetTexture(const std::string& filepath)
{
    if(m_textures.count(filepath))
    {
        return m_textures[filepath];
    }
    else{
        auto ele = new Texture();
        if( ele->Load(filepath) )
        {
            m_textures[filepath] = ele;
        }
        else{
            delete ele;
            ele = nullptr;
        };

        return ele;
    }

}

Mesh* Renderer::GetMesh(const std::string& filepath)
{
    if(m_meshes.count(filepath))
    {
        return m_meshes[filepath];
    }

    auto amesh = new Mesh();
    if( amesh->Load(filepath,this))
    {
        m_meshes[filepath] = amesh; 
    }
    else 
    {
        delete amesh;
        amesh = nullptr;
    };

    return amesh;
}

void Renderer::SetLightUniforms(class Shader* shader)
{
        // camera's transform
        Matrix4 inview = m_viewMat;
        inview.Invert(); 
        
        shader->SetVector3Uniform("u_cameraPos",inview.GetTranslation()); 
        shader->SetVector3Uniform("u_ambientLight",m_ambientLight);
        shader->SetVector3Uniform("u_dirctionLight.m_direction",m_dirctionLight.m_dirction);
        shader->SetVector3Uniform("u_dirctionLight.m_diffuseColor",m_dirctionLight.m_diffuseColor);
        shader->SetVector3Uniform("u_dirctionLight.m_specColor",m_dirctionLight.m_specColor);

}


