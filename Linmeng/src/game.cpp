#include "game.h"

#include "SDL_image.h"

#include "BGSpriteComponent.h"
#include "animSpriteComponent.h"
#include "Ship.h"

#include "tileMapComponent.h"


bool Game::Initialize()
{
    // Sys Backend Init
    {
        if( SDL_Init(SDL_INIT_VIDEO )!=0) // 与sdl_quit 对应
        {
            SDL_Log("sdl video init failed: %s", SDL_GetError());
            return false;
        };

        if( !IMG_Init(IMG_INIT_PNG))
        {
            SDL_Log("sdl imge init failed: %s" , IMG_GetError());
            return false;
        }
    }

    m_window = SDL_CreateWindow( 
        "Linmeng Chapter 1",
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 
        1024, // (int)props.Width, 
        768,  // (int)props.Height,
        SDL_WINDOW_SHOWN |SDL_WINDOW_RESIZABLE);
    if(!m_window)
    {
        SDL_Log("SDL Window Create Failed: %S" , SDL_GetError());
        return false;
    }
    m_renderer = SDL_CreateRenderer(
        m_window,
        -1, // usually -1. make decision by SDL
        SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC );//  SDL_RENDERER_PRESENTVSYNC 提前打开垂直同步
    if( !m_renderer ) 
    {
        SDL_Log("SDL Renderer Create Failed: %s",SDL_GetError());
    }

    LoadData();

    // SDL_GetWindowSize(m_window,&m_data.Width,&m_data.Height);
    // m_data.Width = props.Width;
    // m_data.Height = props.Height;
    // m_data.title = props.Title;
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
    IMG_Quit();

    SDL_Log("Game ShutDown");
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
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

SDL_Texture *Game::GetTexture(const std::string &filepath)
{
    if( m_textures.count(filepath) ) return m_textures[filepath];

    SDL_Texture* newtexture = LoadTexture(filepath);
    if( !newtexture ) 
    {
        SDL_Log("GetTexture faild!:file %s" ,filepath.c_str() );
        return newtexture;
    }
    m_textures[filepath]  = newtexture;

    return newtexture;
}

void Game::LoadData()
{
    m_ship = new Ship(this);
    m_ship->SetPosition({100.f,384.f});
    m_ship->SetScale(1.5f);

    Actor* temp = new Actor(this);
    temp->SetPosition({512.f,384.f});

    // bg 1
    BGSpriteComponent* bg = new BGSpriteComponent(temp,10);
    bg->SetScreenSize({1024.f,768.f});
    std::vector<SDL_Texture*>  bgtexs={
        GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
    };
    bg->SetBGTexture(bgtexs);
    bg->SetScrollSpeed(-100.f);

    // bg 2
    bg = new BGSpriteComponent(temp,50);
    bg->SetScreenSize({1024.f,768.f});
    bgtexs={
        GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
    };
    bg->SetBGTexture(bgtexs);
    bg->SetScrollSpeed(-200.f);

    // tile map
    temp = new Actor(this);
    TileMapComponent* tmap = new TileMapComponent(temp,60);
    
    tmap->SetTileTexture(GetTexture("Assets/Tiles.png"),{8,24});
    tmap->SetTilesMap("Assets/MapLayer2.csv");

}

void Game::UnloadData()
{
    while(! m_actors.empty() )
    {
        ///@attention ~Actor will call RemoveActor() . so it is work.
        delete m_actors.back();
    }    

    for(auto i:m_textures)
    {
        SDL_DestroyTexture(i.second);
    }
    m_textures.clear();

}

SDL_Texture *Game::LoadTexture(const std::string &path)
{


    SDL_Surface* suf = IMG_Load(path.c_str());
    if(!suf)
    {
        SDL_Log("Load %s Failed!: %s",path.c_str(),SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer,suf);
    SDL_FreeSurface(suf);
    if(!texture)
    {
        SDL_Log("Create Texture Failed!: %s",SDL_GetError());
        return nullptr;
    }
    return texture;
}

void Game::AddSprite(SpriteComponent *sprite)
{
    int order = sprite->GetDrawOrder();
    auto it = m_sprites.begin();
    for(;it!=m_sprites.end();it++)
    {
        if(order<=(*it)->GetDrawOrder())
        {
            break;
        }
    }
    m_sprites.insert(it,sprite);
}

void Game::RemoveSprite(SpriteComponent *sprite)
{
    auto it = std::find(m_sprites.begin(),m_sprites.end(),sprite);
    /// cant swap & pop. we need remain the order 
    m_sprites.erase(it);
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

    m_ship->ProccessKeyboard(state);


}



void Game::UpdateGame()
{
    while(!SDL_TICKS_PASSED(SDL_GetTicks() , m_ticksCount + 16 )) ;// wait for 16 ms;
    float deltatime = (SDL_GetTicks() - m_ticksCount)/1000.f;

    if(deltatime > 0.05f)
    {
        deltatime = 0.05f;
    }

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
            delete i;
        }
    }
  
    m_ticksCount = SDL_GetTicks();
}


void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(m_renderer,10,10,20,255);
    SDL_RenderClear(m_renderer);
    
    for(auto i: m_sprites)
    {
        i->Draw(m_renderer);
    }

    SDL_RenderPresent(m_renderer); // backend buffer show(swap)

}
