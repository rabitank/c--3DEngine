#include <fstream>
#include <sstream>

#include "mesh.h"
#include "rapidjson/document.h"
#include  "renderer.h"
#include "texture.h"
#include "shader.h"
#include "vertexArray.h"
#include "SDL.h"

Mesh::Mesh()
    :m_vertexArray(nullptr),
    m_radius(0.f),
    m_specPower(100.f)
{

}

Mesh::~Mesh()
{
    
}

bool Mesh::Load(const std::string& filepath , class Renderer* renderer)
{
    std::ifstream ifs(filepath);
    if(!ifs.is_open())
    {
        SDL_Log("Mesh %s Not found!",filepath);
        return false;
    }

    /// view: rapidJson document
    std::stringstream ss;
    ss <<ifs.rdbuf();
    std::string content = ss.str();
    rapidjson::StringStream jsonstr(content.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonstr);

    if(!doc.IsObject())
    {
        SDL_Log("Mesh %s is not valid json!",filepath.c_str());
        return false;
    }

    int ver = doc["version"].GetInt();
    
    if(ver!=1)
    {
        SDL_Log("Mesh %s is not version 1",filepath.c_str());
        return false;
    }

    // TODO: compile shader read;
    m_shadername = doc["shader"].GetString();

    /// load Texture
    {
        const rapidjson::Value& texture = doc["textures"];
        if(!texture.IsArray() || texture.Size() < 1)
        {   
            SDL_Log("Mesh %s has no Texture",filepath.c_str());
            return false;
        }
        for(rapidjson::size_t i = 0;i<texture.Size(); i++)
        {
            const std::string texturename = texture[i].GetString();

            auto ptr = renderer->GetTexture(texturename);
            if(ptr == nullptr)
            {
                SDL_Log("Mesh %s required Texture %s not exist!" , filepath.c_str(),
                    texturename.c_str());
            }
            m_textures.push_back(ptr);
        };
    }

    /// load VertexArray
    {


        size_t vertsize = 8;
        m_radius = 0.f;

        const rapidjson::Value& vertsJson = doc["vertices"];
        if(!vertsJson.IsArray() || vertsJson.Size() < 1)
        {
            SDL_Log("Mesh %s has no verts data!",filepath.c_str());
            return false;
        }

        /// @attention
        std::vector<float> vertsdata; 
        vertsdata.reserve(vertsJson.Size()*vertsize);

        for(rapidjson::size_t i = 0 ; i<vertsJson.Size() ; i++)
        {
            const rapidjson::Value& avert = vertsJson[i];
            if(!avert.IsArray() || avert.Size() != vertsize)
            {
                SDL_Log("Mesh %s has wrong verts data format!",filepath.c_str());
                return false;
            }

            Vector3 pos(
                avert[0].GetDouble(),
                avert[1].GetDouble(),
                avert[2].GetDouble());
            /// lazy culculate. 这里是长度的平方    
            m_radius  = Math::Max(m_radius , pos.LengthSq());

            for(rapidjson::size_t j = 0;j < vertsize ;j++)
            {
                vertsdata.push_back( 
                    static_cast<float>( avert[j].GetDouble())
                );

            }
        }
        m_radius = Math::Sqrt(m_radius);

        /// load indices
        const rapidjson::Value& indicesJson = doc["indices"];
        if(!indicesJson.IsArray() || indicesJson.Size() < 1 )
        {
            SDL_Log("Mesh %s has no indices data!",filepath.c_str());
            return false;
        }

        /// @attention
        std::vector<unsigned int> indicesdata; 
        indicesdata.reserve(indicesJson.Size() * 3);

        for(rapidjson::size_t i = 0 ; i< indicesJson.Size() ; i++)
        {
            const rapidjson::Value& atriangle = indicesJson[i];
            if(!atriangle.IsArray() || atriangle.Size() != 3)
            {
                SDL_Log("Mesh %s has wrong indices data format!",filepath.c_str());
                return false;
            }

            for(rapidjson::size_t j = 0;j < 3 ;j++)
            {
                indicesdata.push_back( 
                    static_cast<unsigned int>( atriangle[j].GetUint())
                );
            }
        }

        m_vertexArray = new VertexArray(
            vertsdata.data(),static_cast<unsigned int>(vertsdata.size()/vertsize),
            indicesdata.data(),indicesdata.size());
    }

    m_vertexFormat = doc["vertexformat"].GetString();
    m_specPower = static_cast<float>( doc["specularPower"].GetDouble());
    
    return true;
}

void Mesh::Unload()
{
    delete m_vertexArray;
    m_vertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
    if(index>= m_textures.size())
    {
        return nullptr;
    }
    return m_textures[index];

}

