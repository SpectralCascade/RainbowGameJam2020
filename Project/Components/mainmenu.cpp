#include "mainmenu.h"
#include "fitscreen.h"

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

    delta.Init();
}

void MainMenu::Update()
{
    Entity* ghost = entity->Find("Ghost");
    if (ghost != nullptr)
    {
        FitScreen* ghostUI = ghost->GetComponent<FitScreen>();
        if (ghostStartPos == Vector2::Zero)
        {
            ghostStartPos = ghostUI->transformOrigin;
        }

        // Increment animation by a frame
        animPercent += delta.Time() * speed;
        animPercent = std::max(0.0f, animPercent);

        // Actually perform the animation
        ghostUI->transformOrigin = Vector2(
            Tweening::Sine(ghostStartPos.x, ghostStartPos.x + ghostOffset.x, animPercent),
            Tweening::Sine(ghostStartPos.y, ghostStartPos.y + ghostOffset.y, animPercent)
        );

        ghostUI->SetDirty();
    }
    delta.Update();
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
    entity->GetScene()->ClearSafe();
    GetService<ResourceController>()->LoadAndInit<Scene>("assets/scenes/level_1.rawr", entity->GetScene()->GetServices());
    GetService<ResourceController>()->LoadAndInit<Scene>("assets/scenes/UI.rawr", entity->GetScene()->GetServices());
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
