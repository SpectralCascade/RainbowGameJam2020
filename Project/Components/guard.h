#ifndef GUARD_H
#define GUARD_H

#include <Ossium.h>

using namespace Ossium;
using namespace std;

// Forward declaration
class Ghost;
class GuardWaypoint;
class Player;

enum GuardState
{
    GUARD_IDLE = 0,
    GUARD_PATROL,
    GUARD_ALERT,
    GUARD_RUN_AWAY,
    GUARD_SCARED
};

struct GuardSchema : public Schema<GuardSchema, 10>
{
    DECLARE_BASE_SCHEMA(GuardSchema, 10);

    M(string, name) = "Po-po";
    M(float, moveForce) = 18.0f;
    M(float, maxSpeed) = 5.0f;
    M(string, initialWaypoint);
    M(Vector2, direction) = {0, -1};
    M(float, fov) = 135.0f;
    // Rate at which the 'alert' timer cools down each frame, in milliseconds
    M(Uint32, searchTime) = 10;
    // How long it takes before the guard is alerted and it's game over.
    M(Uint32, alertTime) = 500;

};

class Guard : public Component, public GuardSchema
{
public:
    CONSTRUCT_SCHEMA(Component, GuardSchema);
    DECLARE_COMPONENT(Component, Guard);

    void OnLoadFinish();

    void Update();

    float MoveTowards(Vector2 target);

    bool CanSeePlayer();

    // Raycast callback to detect the player
    float32 DetectPlayer(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);

    void Alert();

    GuardState state = GUARD_IDLE;

    vector<Ghost*> ghosts;

private:
    float alertLevel = 0.0f;

    bool canSeePlayer = false;

    PhysicsBody* body = nullptr;

    GuardWaypoint* targetWaypoint = nullptr;

    Player* player = nullptr;

    Timer alertTimer;

    Text* aiText = nullptr;

    Texture* alertBar = nullptr;
    Texture* alertBarBackground = nullptr;

};

#endif // GUARD_H
