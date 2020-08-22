#ifndef GUARD_H
#define GUARD_H

#include <Ossium.h>

using namespace Ossium;
using namespace std;

// Forward declaration
class Ghost;
class GuardWaypoint;

enum GuardState
{
    GUARD_IDLE = 0,
    GUARD_PATROL,
    GUARD_SEARCH,
    GUARD_ATTACK,
    GUARD_RUN_AWAY,
    GUARD_SCARED
};

struct GuardSchema : public Schema<GuardSchema, 5>
{
    DECLARE_BASE_SCHEMA(GuardSchema, 5);

    M(string, name) = "Po-po";
    M(float, moveForce) = 18.0f;
    M(float, maxSpeed) = 5.0f;
    M(string, initialWaypoint);

};

class Guard : public Component, public GuardSchema
{
public:
    CONSTRUCT_SCHEMA(Component, GuardSchema);
    DECLARE_COMPONENT(Component, Guard);

    void OnLoadFinish();

    void Update();

    float MoveTowards(Vector2 target);

    GuardState state = GUARD_IDLE;

    vector<Ghost*> ghosts;

private:
    PhysicsBody* body = nullptr;

    GuardWaypoint* targetWaypoint = nullptr;

};

#endif // GUARD_H
