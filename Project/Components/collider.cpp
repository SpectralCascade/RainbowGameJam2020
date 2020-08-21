#include "collider.h"

// Use OnLoadFinish()
void PhysicsBody::OnLoadFinish()
{
    ParentType::OnLoadFinish();

#ifndef OSSIUM_EDITOR
    Physics::PhysicsWorld* world = entity->GetService<Physics::PhysicsWorld>();
    DEBUG_ASSERT(world != nullptr, "Physics world cannot be NULL");
    Collider* collider = entity->GetComponent<Collider>();
    if (body == nullptr && collider != nullptr)
    {
        // Define the body
        b2BodyDef bodyDef = {
            .position = GetTransform()->GetWorldPosition(),
            .angle = GetTransform()->GetWorldRotation().GetAngle(),
            .type = bodyType,
            .active = IsActiveAndEnabled(),
            .awake = startAwake,
            .allowSleep = allowSleep,
            .angularDamping = angularDamping,
            .angularVelocity = initialAngularVelocity,
            .linearDamping = linearDamping,
            .linearVelocity = initialLinearVelocity,
            .bullet = bullet,
            .fixedRotation = fixedRotation,
            .gravityScale = gravityScale
        }
        // Create the body
        body = world->CreateBody(&bodyDef);
        // Define the fixture
        b2FixtureDef fixDef;
        fixDef.shape = &collider->GetShape();
        fixDef.density = density;
        fixDef.friction = friction;
        fixDef.isSensor = sensor;
        // Create the fixture
        fixture = body->CreateFixture(&fixDef);
        dirty = true;
    }
    // TODO: update properties if body and fixture are already created.
#endif // OSSIUM_EDITOR
}

void PhysicsBody::OnDestroy()
{
    if (body != nullptr)
    {
        Physics::PhysicsWorld* world = entity->GetService<Physics::PhysicsWorld>();
        DEBUG_ASSERT(world != nullptr, "Physics world cannot be NULL");
        // Body will cleanup any attached fixtures
        world->DestroyBody(body);
        body = nullptr;
        fixture = nullptr;
    }
}

void PhysicsBody::UpdatePhysics()
{
    // Update location and rotation in-game.
    const b2Transform& b2t = body->GetTransform();
    Transform* t = GetTransform();
    t->SetWorldPosition(Vector2(b2t.p));
    t->SetWorldRotation(Rotation(b2t.q));
}

//
// Register as an abstract component manually, rather than using macro - so OnLoadFinish() can be overridden.
//

BaseComponent* PhysicsBody::ComponentFactory(void* target_entity)
{
    return nullptr;
}
PhysicsBody::PhysicsBody() {}
PhysicsBody::~PhysicsBody() {}
void PhysicsBody::OnCreate() { PhysicsBody::ParentType::OnCreate(); }
void PhysicsBody::OnDestroy() { PhysicsBody::ParentType::OnDestroy(); }
void PhysicsBody::OnLoadStart() { PhysicsBody::ParentType::OnLoadStart(); }
void PhysicsBody::OnClone(BaseComponent* src) {}
void PhysicsBody::Update(){}
Ossium::TypeSystem::TypeFactory<BaseComponent, ComponentType> PhysicsBody::__ecs_factory_ =
PhysicsBody::ParentType::is_abstract_component ? Ossium::TypeSystem::TypeFactory<BaseComponent, ComponentType>(
    SID( "PhysicsBody" )::str, ComponentFactory, true
) :
Ossium::TypeSystem::TypeFactory<BaseComponent, ComponentType>(
    SID( "PhysicsBody" )::str, ComponentFactory, std::string(parentTypeName), true
);

REGISTER_ABSTRACT_COMPONENT(Collider);
