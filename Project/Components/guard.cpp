#include "guard.h"
#include "guardwaypoint.h"

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
}

void Guard::Update()
{
    switch (state)
    {
    case GUARD_PATROL:
        if (targetWaypoint != nullptr)
        {
            float distance = MoveTowards(targetWaypoint->GetTransform()->GetWorldPosition());
            if (distance < 10.0f)
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
        // Keep an eye out for the player
        break;
    case GUARD_SEARCH:
        break;
    case GUARD_ATTACK:
        break;
    case GUARD_RUN_AWAY:
        break;
    case GUARD_SCARED:
        break;
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
    return differenceToTarget.Length();
}
