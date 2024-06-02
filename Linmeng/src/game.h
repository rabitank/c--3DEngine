
#include "pch.h"
#include "SDL.h" 
#include "vector.h"

#include "soundEvent.h"

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
    class InputSystem* GetInputSystem() {return m_inputSystem;}




private:
    /// three part in Loop
    void ProcessInput() ;
    void UpdateGame()   ;
    void GenerateOutput();

    void HandleKeyDown(int key);

    // temp test
    void ChangeCamera(int mode);

    /// Window created by SDL 😘
    SDL_Window* m_window;    
    
    class Renderer* m_renderer;
    class AudioSystem* m_audioSystem;
    class InputSystem* m_inputSystem;
    


    uint32_t m_ticksCount{0};
    bool m_IsRunning{true};

    /// @brief  game update them in one frame;
    std::vector<class Actor* > m_actors;
    /// @brief game temp sort the actors created when update game. and add them to actors after update;
    std::vector<Actor* > m_pendingActors;
    /// @brief is game updating actors?


    bool m_updatingActors{false};

    class OrbitActor*  mOrbitActor;
	class FollowActor* mFollowActor;
	class FPSActor* mFPSActor;
	// class CameraActor* mSplineActor;

    // class CameraActor* m_camera{nullptr};

    SoundEvent m_musicEvent;
    SoundEvent m_reverbSnap;

    class DebugActor* m_debuger{nullptr};
};

