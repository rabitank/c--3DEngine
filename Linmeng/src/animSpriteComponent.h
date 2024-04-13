#ifndef D78DD07D_CE55_48A4_A321_C03DA6F1A5B0
#define D78DD07D_CE55_48A4_A321_C03DA6F1A5B0


#endif /* D78DD07D_CE55_48A4_A321_C03DA6F1A5B0 */
#pragma once
#include "spriteComponent.h"

#include <vector>

class AnimSpriteComponent:public SpriteComponent
{
    std::vector<class SDL_Texture*> m_animTextures;
    float m_animFps;
    float m_curFrame;

    bool m_loop{true};

public:
    AnimSpriteComponent(class Actor* owner , int DrawOrder = 100):
    SpriteComponent(owner,DrawOrder){};

    virtual void Update(float deltatime) override;

    void SetAnimTextures(const std::vector<class SDL_Texture*>& textures );

    float GetAnimFPS() const{ return m_animFps; };
    AnimSpriteComponent& SetAnimFPS(float fps ) {  m_animFps = fps;return *this; };
    
    bool GetLoop() const { return m_loop;}
    AnimSpriteComponent& SetLoop(bool loop) {m_loop = loop;return *this;};


};