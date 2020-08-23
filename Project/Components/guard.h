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
    GUARD_SEARCH,
    GUARD_ATTACK,
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

    void AttackPlayer();

    GuardState state = GUARD_IDLE;

    vector<Ghost*> ghosts;

private:
    bool canSeePlayer = false;

    PhysicsBody* body = nullptr;

    GuardWaypoint* targetWaypoint = nullptr;

    Player* player = nullptr;

    Timer actionTimer;

    Text* aiText = nullptr;

};

#endif // GUARD_H
