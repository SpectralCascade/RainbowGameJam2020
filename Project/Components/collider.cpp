#include "collider.h"

bool Collider::IsStatic()
{
    return staticBody;
}

void Collider::SetDirty()
{
    dirty = true;
}

void Collider::OnDestroy()
{
    Physics::PhysicsWorld* world = entity->GetService<Physics::PhysicsWorld>();
    DEBUG_ASSERT(world != nullptr, "Physics world cannot be NULL");
    if (body != nullptr)
    {
        // Body will cleanup any attached fixtures
        world->DestroyBody(body);
        body = nullptr;
        fixture = nullptr;
    }
}

void Collider::UpdatePhysics()
{
    const b2Transform& b2t = body->GetTransform();
    Transform* t = GetTransform();
    t->SetWorldPosition(Vector2(b2t.p));
    t->SetWorldRotation(Rotation(b2t.q));
}
