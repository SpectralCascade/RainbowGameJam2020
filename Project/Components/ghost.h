#ifndef GHOST_H
#define GHOST_H

#include <Ossium.h>

using namespace Ossium;

struct GhostSchema : public Schema<GhostSchema, 20>
{
    DECLARE_BASE_SCHEMA(GhostSchema, 20);

    M(float, hitBoxRadius) = 13.0f;
};

class Ghost : public Component, public GhostSchema
{
public:
    DECLARE_COMPONENT(Component, Ghost);

    void OnLoadFinish();

    void Update();

private:
    StateSprite* sprite = nullptr;

};

#endif // GHOST_H
