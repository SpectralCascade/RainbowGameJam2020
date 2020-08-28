#include "ghost.h"
#include "guard.h"

REGISTER_COMPONENT(Ghost);

void Ghost::OnLoadFinish()
{
    sprite = entity->AddComponentOnce<Texture>();
    delta.Init();
    Entity* playerEnt = entity->Find("PlayerController");
    if (playerEnt != nullptr)
    {
        player = playerEnt->GetComponent<Player>();
    }
    DEBUG_ASSERT(player != nullptr, "Player is null! Perhaps it could not be found in the scene?");
    // Parent has the physics body, so this entity can bob up and down
    body = entity->GetParent()->AddComponentOnce<PhysicsBody>();
}

void Ghost::Update()
{
    // Increment animation by a frame
    animPercent += delta.Time() * 0.75f;
    animPercent = std::max(0.0f, animPercent);

    // Bob up and down
    GetTransform()->SetLocalPosition(Vector2(
        GetTransform()->GetLocalPosition().x,
        Tweening::Sine(0, 4.0f, animPercent)
    ));

    switch (state)
    {
    case GHOST_IDLE:
        // Once player is close enough, seek player
        if ((player->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition()).Length() < 80.0f)
        {
            state = GHOST_SEEK_PLAYER;
        }
        break;
    case GHOST_SEEK_PLAYER:
        SeekTarget(player->GetTransform()->GetWorldPosition(), 60);
        break;
    case GHOST_CIRCLE_PLAYER:
        CircleTarget(player->GetTransform()->GetWorldPosition(), Vector2(40, 60));
        break;
    case GHOST_SEEK_GUARD:
        if (guardTarget != nullptr)
        {
            SeekTarget(guardTarget->GetTransform()->GetWorldPosition(), 60);
        }
        else
        {
            state = GHOST_IDLE;
        }
    case GHOST_CIRCLE_GUARD:
        if (guardTarget == nullptr)
        {
            state = GHOST_SEEK_PLAYER;
        }
        else
        {
            CircleTarget(guardTarget->GetTransform()->GetWorldPosition(), Vector2(40, 60));
        }
        break;
    }

    delta.Update();
}

void Ghost::CircleTarget(Vector2 target, Vector2 idealRange)
{
    Vector2 differenceToTarget = (target - GetTransform()->GetWorldPosition());
    float distance = differenceToTarget.Length();
    Vector2 dir = differenceToTarget.Normalized();
    Vector2 force = (dir * moveForce) + (dir.Rotation90Clockwise() * (moveForce * 2.0f));
    if (distance < idealRange.x)
    {
        // Too close, back off a bit
        force += dir * -moveForce;
    }
    else if (distance > idealRange.y)
    {
        // Too far, seek target again
        force += dir * moveForce;
    }
    // Also avoid other ghosts in the scene
    entity->GetScene()->WalkComponents<Ghost>([&] (Ghost* ghost) {
        if (ghost->body != nullptr)
        {
            Vector2 diffToGhost = ghost->body->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
            if (diffToGhost.Length() < 50)
            {
                // Move away from the ghost
                force += diffToGhost.Normalized() * -moveForce * 0.2f;
            }
        }
    });
    body->body->ApplyForceToCenter(force, true);
}

void Ghost::SeekTarget(Vector2 target, float circleDistance)
{
    Vector2 differenceToTarget = (target - GetTransform()->GetWorldPosition());
    // Move towards the player with the same speed
    if (body->body->GetLinearVelocity().LengthSquared() < player->maxSpeed * player->maxSpeed)
    {
        body->body->ApplyForceToCenter(differenceToTarget.Normalized() * player->moveForce, true);
    }
    // If less than 170 pixels away from the player, start circling
    if (differenceToTarget.Length() < circleDistance)
    {
        if (state == GHOST_SEEK_PLAYER)
        {
            state = GHOST_CIRCLE_PLAYER;
            player->ghosts.push_back(this);
        }
        else if (state == GHOST_SEEK_GUARD)
        {
            state = GHOST_CIRCLE_GUARD;
        }
    }
}
