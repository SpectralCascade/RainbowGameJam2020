#ifndef GHOST_H
#define GHOST_H

#include <Ossium.h>
#include "player.h"

using namespace Ossium;

// Forward declaration
class Guard;

struct GhostSchema : public Schema<GhostSchema, 20>
{
    DECLARE_BASE_SCHEMA(GhostSchema, 20);

    M(float, moveForce) = 5.0f;

};

enum GhostState
{
    GHOST_IDLE = 0,
    GHOST_SEEK_PLAYER,
    GHOST_CIRCLE_PLAYER,
    GHOST_SEEK_GUARD,
    GHOST_CIRCLE_GUARD
};

class Ghost : public Component, public GhostSchema
{
public:
    CONSTRUCT_SCHEMA(Component, GhostSchema);
    DECLARE_COMPONENT(Component, Ghost);

    void OnLoadFinish();

    void Update();

    GhostState state = GHOST_IDLE;

    Guard* guardTarget = nullptr;

private:
    void CircleTarget(Vector2 target, Vector2 idealRange);

    void SeekTarget(Vector2 target, float circleDistance);

    Texture* sprite = nullptr;

    Player* player = nullptr;

    Delta delta;

    float animPercent = 0.25f;

    PhysicsBody* body = nullptr;

};

#endif // GHOST_H
