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
        inputGUI->RemoveAll();
        Image* buttonImage = GetService<ResourceController>()->Get<Image>("assets/menu_button.png", *GetService<Renderer>());
        CleanupHandles();
        if (playButton != nullptr)
        {
            playButton->GetEntity()->AddComponentOnce<StateSprite>()->AddState("main", buttonImage, true, 3);
            playClickHandle = playButton->OnClicked += [&] (const Button& caller) { GoPlay(); };
            inputGUI->AddInteractable(context, *playButton);
        }
        else
        {
            Log.Error("Failed to locate play button!");
        }
        if (quitButton != nullptr)
        {
            quitButton->GetEntity()->AddComponentOnce<StateSprite>()->AddState("main", buttonImage, true, 3);
            quitClickHandle = quitButton->OnClicked += [&] (const Button& caller) { Quit(); };
            inputGUI->AddInteractable(context, *quitButton);
        }
        else
        {
            Log.Error("Failed to locate quit button!");
        }
    }
}

void MainMenu::CleanupHandles()
{
    if (playButton != nullptr && playClickHandle >= 0)
    {
        playButton->OnClicked -= playClickHandle;
    }
    if (quitButton != nullptr && quitClickHandle >= 0)
    {
        quitButton->OnClicked -= quitClickHandle;
    }
}

void MainMenu::GoPlay()
{
#ifndef OSSIUM_EDITOR
    // Load the main game scene.
    entity->GetScene()->Load("assets/MainGame.rawr");
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
