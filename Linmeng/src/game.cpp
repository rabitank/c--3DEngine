#include "game.h"
#include <algorithm>

#include "glad/glad.h"

#include "2DComp/spriteComponent.h"
#include "renderer.h"
#include "meshComponent.h"
#include "texture.h"
#include "audioSystem.h" 
#include "inputSystem.h"
#include "audioComponent.h"

#include "debugActor.h"
#include "planeActor.h"

#include "fpsActor.h"
#include "followActor.h"
#include "orbitActor.h"

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
    
    m_inputSystem = new InputSystem();
    if(!m_inputSystem->Initialize())
    {
        SDL_Log("Failed to Init InputSystem");
        delete m_inputSystem;
        m_inputSystem = nullptr;
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
    if(m_audioSystem) m_audioSystem->Shutdown();
    if(m_inputSystem) m_inputSystem->Shutdown();

    SDL_Quit();
}

void Game::AddActor(Actor * newactor)
{
    if(m_updatingActors)
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
    m_debuger = new DebugActor(this);

    Actor* a = new Actor(this);

    //render

    m_renderer->SetAmbientLight(Vector3(0.2f,0.2f,0.2f));
    auto& light =  m_renderer->GetDirectionalLight();
    light.m_diffuseColor = Vector3(0.76f,0.78f,0.82f);
    light.m_dirction = Vector3(0.0f, -0.707f, -0.707f);
    light.m_specColor = Vector3(0.8f, 0.8f, 0.8f);
    

    // Create Cube
    a = new Actor(this);
    a->SetPosition(Vector3 ( 200.f,75.f,0.f));
    a->SetScale(100.f);
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
    
    
	mFPSActor = new FPSActor(this);
	mFollowActor = new FollowActor(this);
	mOrbitActor = new OrbitActor(this);

    ChangeCamera('1');
    

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

    // Create sphere with audio components;
    a = new Actor(this);
    a->SetPosition(Vector3(500.0f, -75.0f, 0.0f));
    a->SetScale(1.0f);
    meshcomp = new MeshComponent(a);
    meshcomp->SetMesh(m_renderer->GetMesh("Assets/Sphere.gpmesh"));
    auto audiocomp = new AudioComponent(a);
    audiocomp->PlayEvent("event:/FireLoop");

    m_musicEvent = m_audioSystem->PlayEvent("event:/Music");

    // if(m_musicEvent.IsValid()) SDL_Log("Music success Load");

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

    m_inputSystem->PrepareForUpdate();

    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_IsRunning = false;
            break;
        case SDL_MOUSEWHEEL:
            m_inputSystem->ProcessEvent(event);
            break;
        case SDL_KEYDOWN:
            HandleKeyDown(event.key.keysym.sym);
            break;
        }
    }

    m_inputSystem->Update();
    const InputState& state = m_inputSystem->GetState();

    
    if(state.keyboard.GetKetValue(SDL_SCANCODE_ESCAPE) )
    {
        m_IsRunning = false;
    }

    m_updatingActors = true;
    {
        for(auto i: m_actors)
        {
            i->ProcessInput(state);
        }
    }
    m_updatingActors = false;
}
void Game::HandleKeyDown(int key)
{
    switch (key)
    {
    case '-' /* constant-expression */:
    {

        float volume =  m_audioSystem->GetBusVolume("bus:/");
        volume = Math::Max(0.f,volume-0.1f);
        m_audioSystem->SetBusVolume("bus:/",volume);
        break;
    }
    case '=' /* constant-expression */:
    {
        SDL_Log("Add bus Volume");
        float volume =  m_audioSystem->GetBusVolume("bus:/");
        SDL_Log("cur bus volume %f",volume);
        volume = volume+0.1f;
        m_audioSystem->SetBusVolume("bus:/",volume);
        break;
    }
    case '1':
	case '2':
	case '3':
	case '4':
		ChangeCamera(key);
		break;
    case 'e' /* constant-expression */:
    {
        m_audioSystem->PlayEvent("event:/Explosion2D");
        break;
    } 
    case 'm' /* constant-expression */:
    {
        m_musicEvent.SetPaused(!m_musicEvent.GetPaused());
        break;
    } 
	case 'r':
		// Stop or start reverb snapshot
		if (!m_reverbSnap.IsValid())
		{   // 回声
			m_reverbSnap = m_audioSystem->PlayEvent("snapshot:/WithReverb");
		}
		else
		{
			m_reverbSnap.Stop();
		}
		break;
	// case '1':
	// 	// Set default footstep surface
	// 	m_camera->SetFootstepSurface(0.0f);
	// 	break;
	// case '2':
	// 	// Set grass footstep surface
	// 	m_camera->SetFootstepSurface(0.5f);
	// 	break; 
    
    default:
        break;
    }
}

void Game::ChangeCamera(int mode)
{

    //TODO: not compile
	// Disable everything
	mFPSActor->SetState(Actor::ePaused);
	mFPSActor->SetVisible(false);
	mFollowActor->SetState(Actor::ePaused);
	mFollowActor->SetVisible(false);
	mOrbitActor->SetState(Actor::ePaused);
	mOrbitActor->SetVisible(false);

	// Enable the camera specified by the mode
	switch (mode)
	{
	case '1':
	default:
		mFPSActor->SetState(Actor::eActive);
		mFPSActor->SetVisible(true);
		break;
	case '2':
		mFollowActor->SetState(Actor::eActive);
		mFollowActor->SetVisible(true);
		break;
	case '3':
		mOrbitActor->SetState(Actor::eActive);
		mOrbitActor->SetVisible(true);
		break;
	// case '4':
	// 	mSplineActor->SetState(Actor::eActive);
	// 	mSplineActor->RestartSpline();
	// 	break;

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

    m_updatingActors = true;
    for(auto i:m_actors)
    {
        i->Update(deltatime);
    }
    m_updatingActors = false;

    for(auto i : m_pendingActors)
    {
        i->ComputeWorldTransform();
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
    
    m_audioSystem->Update(deltatime);
}


void Game::GenerateOutput()
{
    m_renderer -> Draw();
}

