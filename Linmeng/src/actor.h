#pragma once
#include "pch.h"
#include "SDL.h"

/**
 * @brief       具有组件层次结构的游戏对象类. 参照ue4
 * 
 */

#include "vector.h"

class Game;



class Actor
{
public:
    enum State
    {
        eActive,
        ePaused,
        eDead
    };

    /// @brief  依赖注入
    /// @param game 
    Actor(Game* game);
    virtual ~Actor();

    /// @brief  update called from game.
    void Update(float deltatime);

    /// @brief update components 
    /// @param deltatime 
    void UpdateComponents(float deltatime);
    /// @brief specific update code
    virtual void UpdateActor(float deltatime);

    /// @brief processInput and dispear to components
    /// @param states 
    void ProcessInput(const uint8_t* states );

    /// @brief some specific action on actor. 
    /// @attention call after Components Procees in ProcessInput
    virtual void  ActorProcess(const uint8_t* states) {}; 


    Actor& AddComponents(class Component* comp);
    Actor& RemonveComponent(Component* comp);

    State GetState() const { return m_state;};
    Actor& SetState(State state) {m_state = state;return *this;};

    float GetScale() const { return m_scale;};
    Actor& SetScale(float scale)  { m_scale = scale; return *this;};
    
    float GetRotation() const { return m_rotation;};
    Actor& SetRotation(float rot) { m_rotation = rot  ;return *this;};
    
    Vector2 GetPosition() const { return m_position;};

    Actor& SetPosition(Vector2 pos)  { m_position = pos ; return *this;};

    Vector2 GetForward() const;

    Game* GetGame() const { return m_game;}


private:
    // Actor state . related with Update decision
    State m_state;

    Vector2 m_position;
    float m_scale;
    
    // As degree
    float m_rotation;

    std::vector<class Component*> m_components;
    class Game* m_game;
};
