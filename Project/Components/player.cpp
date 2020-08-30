#include "player.h"
#include "ghost.h"

REGISTER_COMPONENT(Player);

void Player::OnCreate()
{
    GetService<InputController>()->AddContext("PlayerInput", &context);
    keyboard = context.GetHandler<KeyboardHandler>();
    mouse = context.GetHandler<MouseHandler>();
    keyboard->AddState("MoveUp", SDLK_UP);
    keyboard->AddState("MoveDown", SDLK_DOWN);
    keyboard->AddState("MoveLeft", SDLK_LEFT);
    keyboard->AddState("MoveRight", SDLK_RIGHT);
}

void Player::OnLoadFinish()
{
    ParentType::OnLoadFinish();
    physicsBody = entity->AddComponentOnce<PhysicsBody>();
    //entity->AddComponentOnce<CircleCollider>();
    playerImage = entity->AddComponentOnce<Texture>();

#ifndef OSSIUM_EDITOR
    Entity* found = nullptr;
    for (auto itr : GetService<ResourceController>()->GetAll<Scene>())
    {
        found = itr.second->Find("GUI");
        if (found != nullptr)
        {
            GameController* gc = found->GetComponent<GameController>();
            if (gc != nullptr)
            {
                // Setup player ghosts
                // TODO
            }
            break;
        }
    }
#endif // OSSIUM_EDITOR

}

void Player::OnDestroy()
{
    GetService<InputController>()->RemoveContext("PlayerInput");
}

void Player::Update()
{
    Vector2 force = Vector2::Zero;
    if (keyboard->GetState("MoveUp"))
    {
        force.y -= moveForce;
    }
    if (keyboard->GetState("MoveDown"))
    {
        force.y += moveForce;
    }
    if (keyboard->GetState("MoveLeft"))
    {
        force.x -= moveForce;
    }
    if (keyboard->GetState("MoveRight"))
    {
        force.x += moveForce;
    }

    if (physicsBody->body != nullptr)
    {
        // Limit velocity
        if (physicsBody->body->GetLinearVelocity().LengthSquared() < maxSpeed * maxSpeed)
        {
            physicsBody->body->ApplyForceToCenter(force, true);
        }

        // Flip texture to show direction of travel
        if (force.x > 0)
        {
            playerImage->SetFlip(SDL_RendererFlip::SDL_FLIP_NONE);
        }
        else if (force.x < 0)
        {
            playerImage->SetFlip(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
        }
    }

}
