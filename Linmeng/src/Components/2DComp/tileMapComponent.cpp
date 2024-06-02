#include "TileMapComponent.h"
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "shader.h"

//TODO: test it
void TileMapComponent::SetTilesMap(const std::string& path )
{
    std::ifstream csv(path,std::ios::in);

    if(!csv.is_open())
    {
        SDL_Log("Can't open csv:%s",path.c_str());
        return;
    }

    m_tilesMap.clear();

    std::string line;
    std::vector<std::string> vs;
    std::string word;

    std::istringstream sin;
    while(getline(csv,line))
    {
        m_tilesMap.push_back({});
        
        word.clear();
        sin.clear();
        sin.str(line);
        while(std::getline(sin,word,','))
        {
            m_tilesMap.back().push_back(atoi(word.c_str()));
        }
    }

    UpdateCell_();

};

void TileMapComponent::SetTileTexture(Texture *texture, Vector2 size)
{
    m_tileTexture.tex = texture;
    m_tileTexture.size = size;

    SetTexture(texture);
}

void TileMapComponent::SetTexture(Texture *texture)
{
    SpriteComponent::SetTexture(texture);
    UpdateCell_();
}

void TileMapComponent::Draw(Shader * shader)
{
    //TODO:  Texure can;t selected Rect to draw. wait 

    // SDL_Rect r;
    // r.w = m_cellSize.x;
    // r.h = m_cellSize.y;

    // SDL_Rect dr;
    // dr.w = m_cellSize.x;
    // dr.h = m_cellSize.y;

    // for(int i = 0;i<m_tilesMap.size();i++)
    // {
    //     for(int j = 0;j<m_tilesMap.front().size();j++)
    //     {
    //         Matrix4 transform = Matrix4::CreateScale(m_cellSize.x);
    //         transform *= Matrix4::CreateTranslation(Vector3());
    //         int index = m_tilesMap[i][j];

    //         r.x = static_cast<int>(index %static_cast<int>( m_tileTexture.size.x) * m_cellSize.x);
    //         r.y = static_cast<int>(index %static_cast<int>( m_tileTexture.size.y) * m_cellSize.y);

    //         dr.x = j * m_cellSize.x;
    //         dr.y = i * m_cellSize.y;

    //     }
    // }

}

void TileMapComponent::UpdateCell_()
{
    m_cellSize.x =  GetTextureWidth() /( m_tileTexture.size.x ? m_tileTexture.size.x : 1.f );
    m_cellSize.y = GetTextureHeight() /( m_tileTexture.size.y ?m_tileTexture.size.y : 1.f );
}

TileMapComponent::TileMapComponent(Actor *owner, int DrawOrder)
:   SpriteComponent(owner,DrawOrder),
    m_tileTexture{nullptr,Vector2(1.f,1.f)},
    m_tilesMap{{}},
    m_cellSize{0,0}
{
}

TileMapComponent::~TileMapComponent(){}
