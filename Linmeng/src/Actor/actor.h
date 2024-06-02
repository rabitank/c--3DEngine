#pragma once
#include "pch.h"
#include "SDL.h"
#include "vector.h"

/**
 * @brief       具有组件层次结构的游戏对象类. 参照ue4
 * 
 */


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
    void UpdateComponents(float deltatime);

    /// @brief specific update code
    virtual void UpdateActor(float deltatime);
    /// @brief some specific action on actor. 
    /// @attention call after Components Procees in ProcessInput
    virtual void  ActorInput( const struct InputState& states) {}; 

    /// @brief processInput and dispear to components
    /// @param states 
    void ProcessInput(const struct InputState& states);



    Actor& AddComponents(class Component* comp);
    Actor& RemonveComponent(Component* comp);

    State GetState() const { return m_state;};
    Actor& SetState(State state) {m_state = state;return *this;};

    const Vector3&      GetPosition() const         { return m_position;};
    Actor&              SetPosition(Vector3 pos)    
    {   m_position = pos ;
        m_recomputeWorldTransform = true;
        return *this;
    };
    float               GetScale() const            { return m_scale;};
    Actor&              SetScale(float scale)       
    { 
        m_scale = scale;
        m_recomputeWorldTransform = true;
        return *this;
    };

    const Quaternion&   GetRotation() const         { return m_rotation;};
    /// @brief the angle is in radiance;
    Actor&              SetRotation(const Quaternion& rot)   
    { 
        m_rotation = rot;
        m_recomputeWorldTransform = true;
        return *this;
    };

    Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX,m_rotation);};
    Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY,m_rotation);};
    
    void ComputeWorldTransform();
    const Matrix4& GetWorldTransform() const {return m_worldTransform;} ;

    Game* GetGame() const { return m_game;}


private:
    // Actor state . related with Update decision
    State m_state;


    Matrix4 m_worldTransform;
    float m_scale;
    Quaternion m_rotation;
    Vector3 m_position;

    std::vector<class Component*> m_components;
    class Game* m_game;
    bool m_recomputeWorldTransform;
};
