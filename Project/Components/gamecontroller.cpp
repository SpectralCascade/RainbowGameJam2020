#include "gamecontroller.h"
#include "popup.h"

REGISTER_COMPONENT(GameController);

void GameController::OnLoadFinish()
{
    ParentType::OnLoadFinish();
    Entity* popupEnt = entity->Find("DialogueBox");
    if (popupEnt != nullptr)
    {
        popup = popupEnt->GetComponent<Popup>();
    }
}
