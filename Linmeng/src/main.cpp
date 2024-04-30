#include "game.h"
#include "SDL.h"

int main(int argc, char** argv)
{

    auto game = new Game();
    
    if(game->Initialize())
    {
        game->RunLoop();
    } ;

    game->ShutDown();
    return 0;

}

