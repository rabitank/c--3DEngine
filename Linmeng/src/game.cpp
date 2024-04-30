#include "game.h"
#include <algorithm>

#include "glad/glad.h"

#include "BGSpriteComponent.h"
#include "animSpriteComponent.h"
#include "cameraActor.h"
#include "renderer.h"
#include "meshComponent.h"
#include "planeActor.h"
#include "texture.h"
#include "audioSystem.h" 

int GAME_Width = 1024;
int GAME_Height = 768;


Game::Game()
    :m_window(nullptr),
    m_renderer(nullptr),
    m_audioSystem(nullptr)
{
}

bool Game::Initialize()
{
    // Sys Backend Init
    {
        if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO )!=0) // 与sdl_quit 对应
        {
            SDL_Log("sdl video init failed: %s", SDL_GetError());
            return false;
        };
    }

    m_renderer = new Renderer(this);
    if( !m_renderer->Initialize(1024.f,768.f))
    {
        SDL_Log("Failed to Init renderer");
        delete m_renderer;
        m_renderer = nullptr;
        return false;
    };

    m_audioSystem = new AudioSystem(this);
    if(!m_audioSystem->Initialize())
    {
        SDL_Log("Failed to Init AudioSystem");
        delete m_audioSystem;
        m_audioSystem = nullptr;
        return false;
    }
    

    LoadData();

    m_ticksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop()
{
    while(m_IsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }

}

void Game::ShutDown()
{
    UnloadData();
    SDL_Log("Game ShutDown");
    if(m_renderer) m_renderer->ShutDown();
    
    SDL_Quit();
}

void Game::AddActor(Actor * newactor)
{
    if(m_UpdatingActors)
    {
        m_pendingActors.push_back(newactor);
    }
    else{
        m_actors.push_back(newactor);
    }

}

void Game::RemoveActor(Actor * actor )
{   
    auto it = std::find(m_actors.begin(),m_actors.end(),actor) ;
    if(it != m_actors.end())
    {
        std::iter_swap(it,m_actors.end()-1);
        m_actors.pop_back();
        return;
        

    }
    it = std::find(m_pendingActors.begin(),m_pendingActors.end(),actor);

    if(it != m_pendingActors.end())
    {
        std::iter_swap(it,m_pendingActors.end()-1);
        m_pendingActors.pop_back();
        return;
    }

    return ;
}


void Game::LoadData()
{
    m_renderer->SetAmbientLight(Vector3(0.2f,0.2f,0.2f));
    auto& light =  m_renderer->GetDirectionalLight();
    light.m_diffuseColor = Vector3(0.76f,0.78f,0.82f);
    light.m_dirction = Vector3(0.0f, -0.707f, -0.707f);
    light.m_specColor = Vector3(0.8f, 0.8f, 0.8f);

    // Create Cube
    Actor* a = new Actor(this);
    a->SetPosition(Vector3 ( 200.f,75.f,0.f));
    a->SetScale(1000.f);
    auto rotQ = Quaternion(Vector3::UnitY,-Math::PiOver2);
    rotQ = Quaternion::Concatenate(
        rotQ,
        Quaternion(Vector3::UnitZ,Math::Pi + Math::Pi/4.f));
    a->SetRotation(rotQ);
    auto meshcomp = new MeshComponent(a);
    meshcomp->SetMesh(m_renderer->GetMesh("Assets/Cube.gpmesh"));
    
    // Create Sphere
    a = new Actor(this);
    a->SetPosition(Vector3 ( 200.f,-75.f,0.f));
    a->SetScale(3.f);
    meshcomp = new MeshComponent(a);
    meshcomp->SetMesh(m_renderer->GetMesh("Assets/Sphere.gpmesh"));

    //floor    
    const float start = -1250.f;
    const float size = 250.f;
    for(int i = 0;i<10;i++)
    {
        for(int j = 0; j<10;j++)
        {
            a = new Plane_A(this);
            a->SetPosition(Vector3(start + i * size , start + j*size,-100.f));
        }
    }
    
    
    m_camera = new CameraActor(this);

    // UI elements
    // spirte pos in ClipWorld is same as pixel pos in window(but 0,0 is window center)
	a = new Actor(this);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(m_renderer->GetTexture("Assets/HealthBar.png"));

	a = new Actor(this);
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(m_renderer->GetTexture("Assets/Radar.png"));

}

void Game::UnloadData()
{
    while(! m_actors.empty() )
    {
        ///@attention ~Actor will call RemoveActor() . so it is work.
        delete m_actors.back();
    }    

    if(m_renderer)
    {
        m_renderer->UnloadData();
    }
}


void Game::ProcessInput()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_IsRunning = false;
            break;
        }
    }

    const uint8_t* state = SDL_GetKeyboardState(nullptr);
    if(state[SDL_SCANCODE_ESCAPE])
    {
        m_IsRunning = false;
    }

    {
        for(auto i: m_actors)
        {
            i->ProcessInput(state);
        }
    }


}


void Game::UpdateGame()
{
    while(!SDL_TICKS_PASSED(SDL_GetTicks() , m_ticksCount + 16 )) ;// wait for 16 ms;
    float deltatime = (SDL_GetTicks() - m_ticksCount)/1000.f;

    if(deltatime > 0.05f)
    {
        deltatime = 0.05f;
    }
    m_ticksCount = SDL_GetTicks();

    m_UpdatingActors = true;
    for(auto i:m_actors)
    {
        i->Update(deltatime);
    }
    m_UpdatingActors = false;

    for(auto i : m_pendingActors)
    {
        m_actors.push_back(i);
    }
    m_pendingActors.clear();

    // into dead process actors
    {
        std::vector<Actor*> deadActors;
        for(auto actor: m_actors)
        {
            if(actor->GetState() == Actor::State::eDead )
            {
                deadActors.push_back(actor);
            }
        }

        for(auto i : deadActors)
        {
            // while ~Actor. the pointer would remove from m_actors;
            delete i;
        }
    }
  
}


void Game::GenerateOutput()
{
    m_renderer -> Draw();
}

