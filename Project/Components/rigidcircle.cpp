#include "rigidcircle.h"

void RigidCircle::OnLoadFinish()
{
    Physics::PhysicsWorld* world = entity->GetService<Physics::PhysicsWorld>();
    DEBUG_ASSERT(world != nullptr, "Physics world cannot be NULL");
#ifdef OSSIUM_EDITOR
    dirty |= (staticBody != _oldStaticBody);
    _oldStaticBody = staticBody;
#endif // OSSIUM_EDITOR
    if (body == nullptr)
    {
        // Create a circle shape
        b2BodyDef bodyDef;
        bodyDef.position.Set(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y);
        if (!staticBody)
        {
            bodyDef.type = b2_dynamicBody;
        }
        body = world->CreateBody(&bodyDef);
        b2FixtureDef fixDef;
        fixDef.shape = &shape;
        fixDef.density = density;
        fixDef.friction = friction;
        fixture = body->CreateFixture(&fixDef);
        dirty = true;
    }
    if (dirty)
    {
        shape.m_radius = radius;
        body->ResetMassData();
    }
}
