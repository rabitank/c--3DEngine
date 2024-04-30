#pragma once
#include "component.h"
#include "soundEvent.h"
#include <vector>
#include <string>

class AudioComponent: public Component
{

public:
    AudioComponent(class Actor* owner , int updateOrder = 200);
    ~AudioComponent();

    void Update(float deltatime) override;
    /// @brief update 3D attribute
    void OnUpdateWorldTransform() override;

    /// @brief call system to add event to audiocomp to play
    /// @param name 
    SoundEvent PlayEvent(const std::string& name);

    void StopAllEvent();

private:

    std::vector<SoundEvent> m_2Devent;
    std::vector<SoundEvent> m_3Devent;

};

