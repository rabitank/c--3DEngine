
#include "pch.h"
#include "SDL.h" 
#include "vector.h"


static constexpr int PaddleLen = 120;
static constexpr int thickness  = 10;

class Game
{
public:
    Game(){};
    /// init 
    bool Initialize();

    /// main loop 
    void RunLoop();
    
    /// close game
    void ShutDown();

    void AddActor(class Actor* );
    void RemoveActor(Actor* );

    
    void LoadData();
    void UnloadData();

    /// @brief GetTexture Call LoadTexture and store in map;
    class SDL_Texture* GetTexture(const std::string& filepath);
    
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    std::vector<class Asteroid* > GetAsteroids() {return m_asteroids;}
    Game& AddAsteroid(Asteroid* as) { m_asteroids.push_back(as) ;return *this;}
    Game& RemoveAsteroid(Asteroid* as);

private:
    /// three part in Loop
    void ProcessInput() ;
    void UpdateGame()   ;
    void GenerateOutput();

    SDL_Texture* LoadTexture(const std::string& path);

    /// Window created by SDL 😘
    SDL_Window* m_window{nullptr};    
    SDL_Renderer* m_renderer{nullptr};

    uint32_t m_ticksCount{0};
    bool m_IsRunning{true};

    std::vector<class SpriteComponent*> m_sprites;

    /// @brief  game update them in one frame;
    std::vector<class Actor* > m_actors;
    /// @brief game temp sort the actors created when update game. and add them to actors after update;
    std::vector<Actor* > m_pendingActors;
    /// @brief is game updating actors?

    std::vector<Asteroid*> m_asteroids;

    bool m_UpdatingActors{false};

    class Ship* m_ship;

    std::unordered_map<std::string , SDL_Texture*> m_textures;


};

