#ifndef PLAYER_H
#define PLAYER_H

#include <Ossium.h>

using namespace Ossium;
using namespace std;

class Ghost;

struct PlayerSchema : public Schema<PlayerSchema, 20>
{
    DECLARE_BASE_SCHEMA(PlayerSchema, 20);

    /// Force applied when moving.
    M(float, moveForce) = 3.0f;

    /// Maximum speed in metres per second
    M(float, maxSpeed) = 5.0f;

};

class Player : public Component, public PlayerSchema
{
public:
    CONSTRUCT_SCHEMA(Component, PlayerSchema);
    DECLARE_COMPONENT(Component, Player);

    void OnCreate();
    void OnDestroy();

    void OnLoadFinish();

    void Update();

    /// Ghosts circling the player
    vector<Ghost*> ghosts;

private:
    // Handy references
    KeyboardHandler* keyboard = nullptr;
    MouseHandler* mouse = nullptr;
    InputContext context;
    PhysicsBody* physicsBody = nullptr;
    Texture* playerImage = nullptr;

};

#endif // PLAYER_H
