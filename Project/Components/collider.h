#ifndef COLLIDER_H
#define COLLIDER_H

#include <Ossium.h>

using namespace Ossium;

struct PhysicsBodySchema : public Schema<PhysicsBodySchema, 20>
{
    DECLARE_BASE_SCHEMA(PhysicsBodySchema, 20);

protected:
    // TODO: make getters and setters for these members as appropriate.

    // Fixture definition variables
    M(float, density) = 1.0f;
    M(float, friction) = 0.3f;
    M(float, restitution) = 0.5f;
    M(bool, sensor) = false;

    // TODO: header attribute (will need to introduce string attributes).

    // Body definition variables
    M(b2_bodyType, bodyType) = b2_dynamicBody;
    M(bool, bullet) = false;
    M(bool, allowSleep) = true;
    M(bool, startAwake) = true;
    M(bool, fixedRotation) = false;
    M(float, gravityScale) = 1.0f;
    M(float, linearDamping) = 0.5f;
    M(float, angularDamping) = 0.5f;

    // Always protected
    M(float, initialLinearVelocity) = { 0.0f, 0.0f };
    M(float, initialAngularVelocity) = { 0.0f, 0.0f };

};

class PhysicsBody : public Component, public PhysicsBodySchema
{
public:
    CONSTRUCT_SCHEMA(Component, PhysicsBodySchema);
    DECLARE_ABSTRACT_COMPONENT(Component, PhysicsBody);

    /// Updates position and rotation transform based on the Box2D body.
    void UpdatePhysics();

protected:
    /// Cleanup physics object from world.
    void OnDestroy();

private:
    // At some point, these will have getters and setters.
    b2Body* body = nullptr;

    // TODO: list of fixtures perhaps, or a dedicated component?
    b2Fixture* fixture = nullptr;

#ifdef OSSIUM_EDITOR
    b2BodyType _editor_oldBodyType = b2_dynamicBody;
#endif // OSSIUM_EDITOR

};

class Collider : public
#ifdef OSSIUM_EDITOR
GraphicComponent
#else
Component
#endif // OSSIUM_EDITOR
{
public:
#ifdef OSSIUM_EDITOR
    DECLARE_ABSTRACT_COMPONENT(GraphicComponent, Collider);
#else
    DECLARE_ABSTRACT_COMPONENT(Component, Collider);
#endif // OSSIUM_EDITOR

    virtual const b2Shape& GetShape() = 0;

};

#endif // COLLIDER_H
