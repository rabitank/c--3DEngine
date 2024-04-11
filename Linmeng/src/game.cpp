#include "game.h"



bool Game::Initialize()
{
    
    if( SDL_Init(SDL_INIT_VIDEO)!=0) // 与sdl_quit 对应
    {
        SDL_Log("sdl video init failed: %s", SDL_GetError());
        return false;
    };

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
    SDL_Log("Game ShutDown");
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
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


    m_paddleDir =0;
    if(state[SDL_SCANCODE_W])
    {
        m_paddleDir = -1;
    }
    if(state[SDL_SCANCODE_S])
    { 
        m_paddleDir = 1;
    }


}


static constexpr int PaddleLen = 120;
static constexpr int thickness  = 10;

void Game::UpdateGame()
{
    while(!SDL_TICKS_PASSED(SDL_GetTicks() , m_ticksCount + 16 )) ;// wait for 16 ms;
    
    
    float deltaTime = (SDL_GetTicks() - m_ticksCount ) / 1000.f;
    if(deltaTime >0.05f)
    {
        deltaTime = 0.05f;
    }
    
    if(m_paddleDir!=0)
    {
        m_paddlePos.y += m_paddleDir * deltaTime * 300.f;
        if(m_paddlePos.y < (PaddleLen / 2 + thickness))
        {
            m_paddlePos.y = PaddleLen / 2 + thickness;
        }
        if(m_paddlePos.y > (768.f - ( PaddleLen / 2 + thickness)))
        {
            m_paddlePos.y = 768.f - ( PaddleLen / 2 + thickness);
        }
    }

    {
        m_ballPos.x += m_ballVec.x*deltaTime;
        m_ballPos.y += m_ballVec.y*deltaTime;

        /// collision with paddle        
        if(m_ballPos.x <= thickness && ( (m_ballPos.y>= m_paddlePos.y - PaddleLen/2 ) && (m_ballPos.y <= m_paddlePos.y + PaddleLen/2 ) ) && m_ballVec.x<0 )
        {
            m_ballVec.x*=-1;
        }
        /// collision with walls        
        if(m_ballPos.x >= 1024 - thickness && m_ballVec.x > 0)
        {
            m_ballVec.x*=-1;
        }
        if(m_ballPos.y >= 768 - thickness && m_ballVec.y > 0 )
        {
            m_ballVec.y*=-1;
        }
        if(m_ballPos.y <= thickness && m_ballVec.y < 0 )
        {
            m_ballVec.y*=-1;
        }

    }

    m_ticksCount = SDL_GetTicks();
}

static void DrawWall()
{

};

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(
        m_renderer,
        0,
        0,
        255,
        255
    );
    SDL_RenderClear(m_renderer);

    SDL_SetRenderDrawColor(
        m_renderer,
        255,
        255,
        255,
        255
    );



    SDL_Rect wall{
        0, // top left x
        0, // tl y
        1024,// w
        thickness //h
    };
    SDL_Rect ball{
        static_cast<int> (m_ballPos.x - thickness/2 ),
        static_cast<int> (m_ballPos.y - thickness/2 ), 
        thickness,
        thickness
    };

    SDL_Rect paddle{
        static_cast<int> (m_paddlePos.x - thickness/2 ),
        static_cast<int> (m_paddlePos.y - PaddleLen/2 ), 
        thickness,
        PaddleLen,
    };


    SDL_RenderFillRect(m_renderer,&wall);
    wall.y = 768-thickness;
    SDL_RenderFillRect(m_renderer,&wall);
    wall.y = 0;
    wall.x = 1024-thickness;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(m_renderer,&wall);
    
    SDL_RenderFillRect(m_renderer,&ball);
    SDL_RenderFillRect(m_renderer,&paddle);

    SDL_RenderPresent(m_renderer); // backend buffer show(swap)

}
