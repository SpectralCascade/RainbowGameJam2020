#ifndef COLLIDER_H
#define COLLIDER_H

#include <Ossium.h>

using namespace Ossium;

struct ColliderSchema : public Schema<ColliderSchema, 20>
{
    DECLARE_BASE_SCHEMA(ColliderSchema, 20);

    M(float, density) = 1.0f;
    M(float, friction) = 0.3f;

protected:
    M(bool, staticBody) = false;

};

class Collider : public Component, public ColliderSchema
{
public:
    CONSTRUCT_SCHEMA(Component, ColliderSchema);
    DECLARE_COMPONENT(Component, Collider);

    /// Is this a static collider, or dynamic?
    bool IsStatic();

    /// Set this collider dirty. Called when toggled between static or dynamic body.
    void SetDirty();

    /// Cleanup physics object from world.
    void OnDestroy();

    /// Updates position and rotation transform based on the body.
    void UpdatePhysics();

    /// Called after the Update() method.
    void PostUpdate();

protected:
    bool dirty = true;

    b2Body* body = nullptr;

    b2Fixture* fixture = nullptr;

#ifdef OSSIUM_EDITOR
    bool _oldStaticBody = false;
#endif // OSSIUM_EDITOR

};

#endif // COLLIDER_H
