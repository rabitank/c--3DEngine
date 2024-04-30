
#include "pch.h"
#include "SDL.h" 
#include "vector.h"



extern int GAME_Width  ;
extern int GAME_Height ;



class Game
{
public:
    Game();
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
    
    class Renderer* GetRenderer() {return m_renderer;}
    class AudioSystem* GetAudioSystem() {return m_audioSystem;}



private:
    /// three part in Loop
    void ProcessInput() ;
    void UpdateGame()   ;
    void GenerateOutput();


    /// Window created by SDL 😘
    SDL_Window* m_window;    
    
    class Renderer* m_renderer;
    class AudioSystem* m_audioSystem;

    uint32_t m_ticksCount{0};
    bool m_IsRunning{true};

    /// @brief  game update them in one frame;
    std::vector<class Actor* > m_actors;
    /// @brief game temp sort the actors created when update game. and add them to actors after update;
    std::vector<Actor* > m_pendingActors;
    /// @brief is game updating actors?


    bool m_UpdatingActors{false};

    class CameraActor* m_camera{nullptr};


};

