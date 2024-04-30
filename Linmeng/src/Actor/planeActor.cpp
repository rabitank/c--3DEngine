#include "planeActor.h"
#include "game.h"
#include "renderer.h"
#include "meshComponent.h"

Plane_A::Plane_A(class Game* game)
    :Actor(game)
{
    auto amesh = game->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
    auto ameshcomp = new MeshComponent(this);
    ameshcomp->SetMesh(amesh);
}
