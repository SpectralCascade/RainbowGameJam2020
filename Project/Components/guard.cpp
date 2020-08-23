#include "guard.h"
#include "guardwaypoint.h"
#include "player.h"

using namespace Ossium;

REGISTER_COMPONENT(Guard);

void Guard::OnLoadFinish()
{
    body = entity->AddComponentOnce<PhysicsBody>();
    if (!initialWaypoint.empty() && targetWaypoint == nullptr)
    {
        // Find the waypoint
        Entity* target = entity->Find(initialWaypoint);
        targetWaypoint = target != nullptr ? target->GetComponent<GuardWaypoint>() : nullptr;
        if (targetWaypoint != nullptr)
        {
            state = GUARD_PATROL;
        }
    }
    Entity* playerEnt = entity->Find("PlayerController");
    if (playerEnt != nullptr)
    {
        player = playerEnt->GetComponent<Player>();
    }
    Entity* textEnt = entity->Find("text", entity);
    if (textEnt != nullptr)
    {
        aiText = textEnt->GetComponent<Text>();
    }
    actionTimer.Stop();
}

bool Guard::CanSeePlayer()
{
    return canSeePlayer;
}

void Guard::Update()
{
    Physics::PhysicsWorld* world = GetService<Physics::PhysicsWorld>();
    DEBUG_ASSERT(world != nullptr && player != nullptr, "PLAYER and PHYSICS WORLD should not be NULL!");

    Vector2 playerDiff = player->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();

    // Keep an "eye" out for the player...
    canSeePlayer = false;
    float angleToPlayer = playerDiff.Rotation();
    // Is the player in front of us?
    // If so, try and raycast to the player. If blocked, cannot see the player, so do nothing.
    if (direction.Rotation() > angleToPlayer - (fov / 2.0f) && direction.Rotation() < angleToPlayer + (fov / 2.0f))
    {
        Vector2 origin = GetTransform()->GetWorldPosition();
        // Convert to metres
        origin.x = PTM(origin.x);
        origin.y = PTM(origin.y);

        world->RayCast(
            Ray(origin, playerDiff.Normalized()),
            Physics::OnRayHit([&] (b2Fixture* fixture, const Vector2& point, const Vector2& normal, float32 fraction) {
                if (fixture->GetBody() == body->body)
                {
                    // Ignore own body
                    return -1.0f;
                }
                else if (fixture->GetUserData() != nullptr)
                {
                    Collider* collider = (Collider*)fixture->GetUserData();
                    // Is the collider the player or not?
                    canSeePlayer = collider->GetEntity()->GetComponent<Player>() == player;
                }
                // Terminate raycast
                return 0.0f;
            })
        );

    }

    switch (state)
    {
    case GUARD_PATROL:
        if (targetWaypoint != nullptr)
        {
            float distance = MoveTowards(targetWaypoint->GetTransform()->GetWorldPosition());
            if (distance < 20.0f)
            {
                // Get next target
                targetWaypoint = targetWaypoint->GetNextWaypoint();
                if (targetWaypoint == nullptr)
                {
                    state = GUARD_IDLE;
                }
            }
        }
    case GUARD_IDLE:
        if (canSeePlayer)
        {
            // Go after the player! Pretty dumb though... need some path finding.
            AttackPlayer();
        }
        break;
    case GUARD_SEARCH:
        // Search for 10 seconds after losing sight of the player.
        if (actionTimer.GetTicks() > 10000)
        {
            if (targetWaypoint != nullptr)
            {
                // TODO: Find way back to waypoint.
                state = GUARD_PATROL;
            }
            else
            {
                state = GUARD_IDLE;
            }
            if (aiText != nullptr)
            {
                aiText->text = " ";
                aiText->layout.mainColor = Colors::TRANSPARENT;
                aiText->dirty = true;
            }
        }
        else if (canSeePlayer)
        {
            // Player spotted again.
            AttackPlayer();
        }
        break;
    case GUARD_ATTACK:
        if (!canSeePlayer)
        {
            // Can no longer see player, start searching
            actionTimer.Start();
            state = GUARD_SEARCH;
            if (aiText != nullptr && aiText->text != "?")
            {
                aiText->text = "?";
                aiText->layout.mainColor = Colors::YELLOW;
                aiText->dirty = true;
            }
        }
        else
        {
            // Move towards the player.
            MoveTowards(player->GetTransform()->GetWorldPosition());
        }
        break;
    case GUARD_RUN_AWAY:
        // TODO
        break;
    case GUARD_SCARED:
        // TODO
        break;
    }
}

void Guard::AttackPlayer()
{
    state = GUARD_ATTACK;
    if (aiText != nullptr && aiText->text != "!")
    {
        aiText->text = "!";
        aiText->layout.mainColor = Colors::RED;
        aiText->dirty = true;
    }
}

float Guard::MoveTowards(Vector2 target)
{
    Vector2 differenceToTarget = (target - GetTransform()->GetWorldPosition());
    // Move towards the player with the same speed
    if (body->body->GetLinearVelocity().LengthSquared() < maxSpeed * maxSpeed)
    {
        body->body->ApplyForceToCenter(differenceToTarget.Normalized() * moveForce, true);
    }
    direction = differenceToTarget.Normalized();
    return differenceToTarget.Length();
}
