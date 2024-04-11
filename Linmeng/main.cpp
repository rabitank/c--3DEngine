#include "game.h"

int main(int, char**){
    
    std::cout<< "Hello, from Linmeng!\n";
    
    Game game;
    bool success = game.Initialize();
    if(success)
        game.RunLoop();
    game.ShutDown();

    return 0;
}
