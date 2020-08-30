#include "global.h"
#include "popup.h"
#include "gamecontroller.h"

Ossium::Scene* SceneMenu = nullptr;

Ossium::Scene* SceneUI = nullptr;

Ossium::Scene* SceneWorld = nullptr;

bool EnteredHub = false;
bool EnteredLevel2 = false;
bool EnteredLevel3 = false;
bool EnteredExit = false;

int totalGhosts = 0;

Popup* FindPopup()
{
    if (SceneUI != nullptr)
    {
        Entity* gui = SceneUI->Find("GUI");
        if (gui != nullptr)
        {
            GameController* gc = gui->GetComponent<GameController>();
            if (gc != nullptr)
            {
                return gc->popup;
            }
        }
    }
    return nullptr;
}
