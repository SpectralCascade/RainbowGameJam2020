#include "guard.h"
#include "guardwaypoint.h"
#include "player.h"
#include "gamecontroller.h"

using namespace Ossium;
using namespace std;

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
    Entity* found = entity->Find("PlayerController");
    if (found != nullptr)
    {
        player = found->GetComponent<Player>();
    }
    found = entity->Find("text", entity);
    if (found != nullptr)
    {
        aiText = found->GetComponent<Text>();
    }
    found = entity->Find("alertFill", entity);
    if (found != nullptr)
    {
        alertBar = found->GetComponent<Texture>();
    }
    found = entity->Find("alertFillBG", entity);
    if (found != nullptr)
    {
        alertBar = found->GetComponent<Texture>();
    }
    alertTimer.Stop();
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
        Point origin(PTM(GetTransform()->GetWorldPosition()));
        Point playerPosMetres(PTM(player->GetTransform()->GetWorldPosition()));

        // As soon as a collider is hit which is *not* the player,
        canSeePlayer = true;
        Physics::OnRayHit onHit = Physics::OnRayHit([&] (b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
            if (fixture->GetBody() == body->body)
            {
                // Ignore own body
                return -1.0f;
            }
            Collider* hit = (Collider*)fixture->GetUserData();
            if (hit != nullptr && hit->GetEntity() != player->GetEntity())
            {
                // Terminate raycast, there's something between this guard and the player blocking line of sight.
                canSeePlayer = false;
                return 0.0f;
            }
            // Ignore, probably hit the player.
            return -1.0f;
        });

        // Raycast directly from the fixture to the player.
        world->RayCast(origin, playerPosMetres, &onHit);

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
        if (!alertTimer.IsPaused() && alertTimer.GetTicks() >= searchTime)
        {
            if (targetWaypoint != nullptr)
            {
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
            alertTimer.Pause();
            alertLevel = 0;
        }
        else if (canSeePlayer)
        {
            // Player spotted again.
            Alert();
        }
        else
        {
            alertLevel = Utilities::Clamp(((float)searchTime - (float)alertTimer.GetTicks()) / (float)searchTime);
        }
        break;
    case GUARD_ALERT:
        if (!canSeePlayer)
        {
            // Can no longer see player, start searching
            alertTimer.Start();
            state = targetWaypoint != nullptr ? GUARD_PATROL : GUARD_IDLE;
            if (aiText != nullptr && aiText->text != "?")
            {
                aiText->text = "?";
                aiText->layout.mainColor = Colors::YELLOW;
                aiText->dirty = true;
            }
        }
        else
        {
            // Increase alertness
            alertLevel = Utilities::Clamp((float)alertTimer.GetTicks() / (float)alertTime);
            if (alertLevel >= 1.0f)
            {
                // Find the game controller
                for (auto itr : GetService<ResourceController>()->GetAll<Scene>())
                {
                    Scene* scene = (Scene*)itr.second;
                    if (scene != entity->GetScene())
                    {
                        Entity* found = scene->Find("GameController");
                        if (found != nullptr)
                        {
                            found->GetComponent<GameController>()->GameOver();
                        }
                    }
                }
            }
        }
        break;
    case GUARD_RUN_AWAY:
        // TODO
        break;
    case GUARD_SCARED:
        // TODO
        break;
    }

    if (alertBar != nullptr)
    {
        alertBar->SetRenderWidth(alertLevel);
        if (alertBarBackground != nullptr)
        {
            alertBarBackground->SetRenderWidth(alertLevel > 0.0f ? 1.0f : 0.0f);
        }
    }

}

void Guard::Alert()
{
    alertTimer.Start();
    state = GUARD_ALERT;
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
    direction = Vector2(body->body->GetLinearVelocity()).Normalized();
    return differenceToTarget.Length();
}
