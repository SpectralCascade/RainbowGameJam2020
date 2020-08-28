#include "gamecontroller.h"
#include "popup.h"

REGISTER_COMPONENT(GameController);

void GameController::OnCreate()
{
    GetService<InputController>()->AddContext("GameControllerInput", &gameInput);
    mouse = gameInput.AddHandler<MouseHandler>();
}

void GameController::OnDestroy()
{
    GetService<InputController>()->RemoveContext("GameControllerInput");
}

void GameController::OnLoadFinish()
{
    ParentType::OnLoadFinish();
    Entity* popupEnt = entity->Find("DialogueBox");
    if (popupEnt != nullptr)
    {
        popup = popupEnt->GetComponent<Popup>();
    }
#ifndef OSSIUM_EDITOR
    if (popup != nullptr)
    {
        popup->QueueMessage("Hello there! How does this look in the text box? Lorem ipsum dolor set amet bla bla bla...");
        popup->QueueMessage("This is the second message. Hopefully when you clicked this showed up in a similar fashion. Click again to hide the popup!");
        popup->NextMessage();
    }
#endif // OSSIUM_EDITOR
}

void GameController::Update()
{
    if (!mouseWasPressed && mouse->LeftPressed() && popup != nullptr)
    {
        popup->NextMessage();
    }
    // Freeze physics
    GetService<PhysicsWorld>()->SetFrozen(popup->IsShown());

    mouseWasPressed = mouse->LeftPressed();
}
