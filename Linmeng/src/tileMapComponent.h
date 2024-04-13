
#pragma once


#include "spriteComponent.h"
#include <vector>
#include <string>
#include "vector.h"

class TileMapComponent:public SpriteComponent
{
    struct TileTexture
    {
        SDL_Texture* tex;
        Vector2 size;
    };

    std::vector<std::vector<int>> m_tilesMap;
    TileTexture m_tileTexture;
    
    Vector2 m_cellSize;


public:
    TileMapComponent(class Actor* owner , int DrawOrder);
    ~TileMapComponent();

    void SetTilesMap(const std::string& path );

    void SetTileTexture(SDL_Texture* texture,Vector2 size );

    
    void Draw(class SDL_Renderer* renderer) override;

    const std::vector<std::vector<int>>& GetTiles() const {return m_tilesMap;};

private:
    /// @attention changed access authority. from public to private . view: https://blog.csdn.net/t_t233333333/article/details/103411507
    /// @param texture 
    void SetTexture(SDL_Texture* texture) override;
    void UpdateCell_();
};
