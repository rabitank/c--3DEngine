#include "fmod.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include "SDL_log.h"
#include "SDL.h"

// view: https://www.fmod.com/docs/2.03/api/studio-guide.html  -> update

#define logcall(x , y) result = x;\
    if(result!=FMOD_OK) \
    {\
        SDL_Log(y);  \
        return 0;\
    }


int main(int argc, char** argv)
{
    FMOD::Studio::System* sys;
    FMOD_RESULT result=FMOD::Studio::System::create(&sys);
    if(result!=FMOD_OK){
        SDL_Log("sys create failed ");                
        return 0;
    }
    
    result= sys->initialize( 1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
    if(result!=FMOD_OK){
        SDL_Log("sys init failed ");                
        return 0;
    }

    FMOD::System* coresys;
    sys->getCoreSystem(&coresys);
    coresys->setSoftwareChannels(8);
    coresys->setSoftwareFormat( 48000,FMOD_SPEAKERMODE_RAW,2);
    

    sys->setNumListeners(1);
    
    FMOD::Studio::Bank* bank1;
    FMOD::Studio::Bank* bank2;

    logcall( sys->loadBankFile("Assets/AnMaster.strings.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&bank1),"bank load failed");
    logcall( sys->loadBankFile("Assets/AnMaster.bank",FMOD_STUDIO_LOAD_BANK_NORMAL,&bank2) ,"bank load failed");

    FMOD::Studio::EventDescription* descript;
    logcall (sys->getEvent("event:/bgm003",&descript),"failed get event");
    FMOD::Studio::EventInstance* ins;
    logcall( descript->createInstance(&ins) , "event create ins failed");
    bool is3d = true;
    descript->is3D(&is3d);
    if(!is3d)    SDL_Log("is not 3D");

    logcall( ins->start() , "ins start failed");

    /// 更新, 将队列指令提交到后端(即以上啥啥的)
    sys->update();

    while(!SDL_TICKS_PASSED(SDL_GetTicks() ,  16000 ));

    return 0;

}