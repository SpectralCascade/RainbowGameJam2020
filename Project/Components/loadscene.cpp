#include "loadscene.h"
#include "global.h"
#include "popup.h"
#include "gamecontroller.h"

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
                if (sceneToLoad == "assets/scenes/MainMenu.rawr")
                {
                    SceneMenu->LoadSafe(sceneToLoad);
                }
                else
                {
                    if (SceneWorld == nullptr)
                    {
                        for (auto itr : GetService<ResourceController>()->GetAll<Scene>())
                        {
                            Scene* scene = (Scene*)itr.second;
                            if (scene != SceneMenu && scene != SceneUI)
                            {
                                SceneWorld = scene;
                                break;
                            }
                        }
                    }
                    if (sceneToLoad == "Win game!")
                    {
                        GameController* gc = nullptr;
                        if (SceneUI != nullptr)
                        {
                            Entity* gui = SceneUI->Find("GUI");
                            if (gui != nullptr)
                            {
                                gc = gui->GetComponent<GameController>();
                            }
                        }
                        if (gc != nullptr)
                        {
                            // Win game
                            gc->GameOver(true);
                        }

                        SceneWorld->ClearSafe();
                        break;
                    }
                    else
                    {
                        SceneWorld->LoadSafe(sceneToLoad);
                    }

                    Popup* popup = FindPopup();

                    if (popup != nullptr)
                    {
                        if (sceneToLoad == "assets/scenes/hub.rawr" && !EnteredHub)
                        {
                            EnteredHub = true;
                            popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> Huh...? Where's everyone gone??", "", "assets/gayghost.png");
                            popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> We'll have to find them. There's no way you can escape unless we hide you from the guards...", "", "assets/gayghost.png");
                            popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> They can't have gone too far. Let's check out the nearby rooms.", "", "assets/gayghost.png");
                            popup->NextMessage();
                        }
                        else if (sceneToLoad == "assets/scenes/exit.rawr")
                        {
                            // TODO: check player has all the ghosts
                            if (!EnteredExit)
                            {
                                EnteredExit = true;
                                popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> Hey wait up! That's the exit - there's no way past those guards though. We need more spirits!", "", "assets/gayghost.png");
                                popup->QueueMessage("<color=#7F00FF><b>You:</b></color> I think I'll pass on the whiskey thanks.", "assets/playerHead.png");
                                popup->QueueMessage("You hear the ghost let out a sigh...", "", "assets/gayghost.png");
                                popup->NextMessage();
                            }
                        }
                        else if (sceneToLoad == "assets/scenes/level_2.rawr" && !EnteredLevel2)
                        {
                            EnteredLevel2 = true;
                            popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> Oh, I can see and Mark and Barney in the corner. GUYS, we're ready to go!", "", "assets/gayghost.png");
                            popup->QueueMessage("<color=#00AAAA><b>Mark & Barney:</b></color> *smooching*");
                            popup->QueueMessage("<color=#7F00FF><b>You:</b></color> I don't think they heard us... looks like I'll have to try and get their attention.", "assets/playerHead.png");
                            popup->NextMessage();
                        }
                        else if (sceneToLoad == "assets/scenes/level_3.rawr" && !EnteredLevel3)
                        {
                            EnteredLevel3 = true;
                            popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> Huh, just two guards. All our pals are evenly balanced between them.", "", "assets/gayghost.png");
                            popup->QueueMessage("<color=#7F00FF><b>You:</b></color> I guess this is an advanced spirit level...", "assets/playerHead.png");
                            popup->NextMessage();
                        }
                    }
                }
                break;
            }
        }
    }
}
