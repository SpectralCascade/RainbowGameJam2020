#include "loadscene.h"

REGISTER_COMPONENT(LoadScene);

void LoadScene::OnLoadFinish()
{
#ifndef OSSIUM_EDITOR
    Entity* playerEnt = entity->Find("PlayerController");
    if (playerEnt != nullptr)
    {
        playerCollider = playerEnt->GetComponent<Collider>();
    }
    sensor = entity->GetComponent<PhysicsBody>();
#endif // OSSIUM_EDITOR
}

void LoadScene::Update()
{
    if (playerCollider != nullptr && sensor != nullptr)
    {
        for (b2ContactEdge* ce = sensor->body->GetContactList(); ce; ce = ce->next)
        {
            b2Contact* contact = ce->contact;
            if (contact->GetFixtureA()->GetUserData() == playerCollider || contact->GetFixtureB()->GetUserData() == playerCollider)
            {
                // Touching the player, load the next scene
                entity->GetScene()->LoadSafe(sceneToLoad);
                break;
            }
        }
    }
}
