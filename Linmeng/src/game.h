
#include "pch.h"
#include "SDL.h" 
#include "vector.h"

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
private:
    /// three part in Loop
    void ProcessInput() ;
    void UpdateGame()   ;
    void GenerateOutput();

    /// Window created by SDL 😘
    SDL_Window* m_window{nullptr};    
    SDL_Renderer* m_renderer{nullptr};

    bool m_IsRunning{true};
    
    Vector2 m_ballPos{500,500};
    Vector2 m_ballVec{-200.f,235.f};


    Vector2 m_paddlePos{0,500};
    int m_paddleDir = 0;

    uint32_t m_ticksCount{0};


};

