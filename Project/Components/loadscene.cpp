#include "loadscene.h"

void LoadScene::OnLoadFinish()
{
#ifndef OSSIUM_EDITOR
    Entity* playerEnt = entity->Find("PlayerController");
    if (playerEnt != nullptr)
    {
        playerCollider = playerEnt->GetComponent<Collider>();
    }
    sensor = GetComponent<PhysicsBody>();
#endif // OSSIUM_EDITOR
}

void LoadScene::Update()
{
    if (playerCollider != nullptr)
    {
        for (b2ContactEdge* ce = myBody->GetContactList(); ce; ce = ce->next)
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
