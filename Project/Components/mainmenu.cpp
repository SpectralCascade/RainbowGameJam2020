#include "mainmenu.h"

REGISTER_COMPONENT(MainMenu);

void MainMenu::OnLoadFinish()
{
    Component::OnLoadFinish();
    // Component references *did* work. But now they don't. So I'll do this instead :D
    inputGUI = entity->AddComponentOnce<InputGUI>();
    Entity* target = entity->GetScene()->Find("Play");
    playButton = target != nullptr ? target->GetComponent<Button>() : nullptr;
    target = entity->GetScene()->Find("Quit");
    quitButton = target != nullptr ? target->GetComponent<Button>() : nullptr;

    if (inputGUI != nullptr)
    {
        const char* context = SID("MainMenu")::str;
        if (GetService<InputController>()->GetContext(context) == nullptr)
        {
            GetService<InputController>()->AddContext(context, inputGUI);
        }
        inputGUI->RemoveAll();
        Image* buttonImage = GetService<ResourceController>()->Get<Image>("assets/menu_button.png", *GetService<Renderer>());
        if (playButton != nullptr)
        {
            playButton->GetEntity()->AddComponentOnce<StateSprite>()->AddState("main", buttonImage, true, 3);
            inputGUI->AddInteractable(context, *playButton);
        }
        else
        {
            Log.Error("Failed to locate play button!");
        }
        if (quitButton != nullptr)
        {
            quitButton->GetEntity()->AddComponentOnce<StateSprite>()->AddState("main", buttonImage, true, 3);
            inputGUI->AddInteractable(context, *quitButton);
        }
        else
        {
            Log.Error("Failed to locate quit button!");
        }
    }
}

void MainMenu::GoPlay()
{
#ifndef OSSIUM_EDITOR
    // Load the main game scene.
    entity->GetScene()->Load("assets/Game.rawr");
#else
    Log.Info("Play button pressed!");
#endif // OSSIUM_EDITOR
}

void MainMenu::Quit()
{
#ifndef OSSIUM_EDITOR
    // Quit the application.
    // TODO
#else
    Log.Info("Quit button pressed!");
#endif // OSSIUM_EDITOR
}
