#include "gamecontroller.h"
#include "popup.h"
#include "global.h"

REGISTER_COMPONENT(GameController);

void GameController::OnDestroy()
{
    GetService<InputController>()->RemoveContext("PopupContext");
}

void GameController::OnLoadFinish()
{
    ParentType::OnLoadFinish();

    totalGhosts = 0;

    // Hook up components
    Entity* found = entity->Find("DialogueBox");
    popup = found != nullptr ? found->GetComponent<Popup>() : nullptr;
    gui = entity->GetComponent<InputGUI>();
    found = entity->Find("QuitButton");
    quitToMainMenuButton = found != nullptr ? found->GetComponent<Button>() : nullptr;
    found = entity->Find("RestartButton");
    restartLevelButton = found != nullptr ? found->GetComponent<Button>() : nullptr;
    found = entity->Find("GameOverText");
    gameOverText = found != nullptr ? found->GetComponent<Text>() : nullptr;

#ifndef OSSIUM_EDITOR
    // Setup input
    if (gui != nullptr)
    {
        const char* context = SID("GameControllerInput")::str;
        gui->RemoveAll();
        Image* buttonImage = GetService<ResourceController>()->Get<Image>("assets/menu_button.png", *GetService<Renderer>());
        CleanupHandles();
        if (restartLevelButton != nullptr)
        {
            restartLevelButton->GetEntity()->AddComponentOnce<StateSprite>()->AddState("main", buttonImage, true, 3);
            playClickHandle = restartLevelButton->OnClicked += [&] (const Button& caller) { RestartLevel(); };
            gui->AddInteractable(context, *restartLevelButton);
        }
        else
        {
            Log.Error("Failed to locate play button!");
        }
        if (quitToMainMenuButton != nullptr)
        {
            quitToMainMenuButton->GetEntity()->AddComponentOnce<StateSprite>()->AddState("main", buttonImage, true, 3);
            quitClickHandle = quitToMainMenuButton->OnClicked += [&] (const Button& caller) { QuitToMainMenu(); };
            gui->AddInteractable(context, *quitToMainMenuButton);
        }
        else
        {
            Log.Error("Failed to locate quit button!");
        }

        GetService<InputController>()->AddContext("PopupContext", &popupContext);

        // Separate context for the popup
        mouse = popupContext.GetHandler<MouseHandler>();
    }

    // Setup start popup
    if (popup != nullptr)
    {
        popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> Hey, you. You're finally awake.", "", "assets/gayghost.png");
        popup->QueueMessage("<color=#7F00FF><b>You:</b></color> Huh..? Wha... who are you? Where am I?!", "assets/playerHead.png");
        popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> It's me! We met at spirit con!? But uh... we got busted.", "", "assets/gayghost.png");
        popup->QueueMessage("<color=#7F00FF><b>You:</b></color> Oh, I recall now... how did you get here?", "assets/playerHead.png");
        popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> They built this place on top of a resting ground... so it wasn't too difficult to find you!", "", "assets/gayghost.png");
        popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> We've gotta get you outta here anyway. Some friends are with me in the next room - we'll get you to safety.", "", "assets/gayghost.png");
        popup->QueueMessage("<color=#00AA00><b>Ghost:</b></color> Just be sure not to let the guard catch you on your way out!", "", "assets/gayghost.png");
        popup->QueueMessage("Use the arrow keys to move. Avoid guards, or you'll be caught and have to try again!");
        popup->NextMessage();
    }
    if (!nextLevel.empty())
    {
        SceneWorld = GetService<ResourceController>()->Get<Scene>(nextLevel, entity->GetScene()->GetServices());
    }

    HideGameOver();
#endif // OSSIUM_EDITOR
}

void GameController::CleanupHandles()
{
    if (restartLevelButton != nullptr && playClickHandle >= 0)
    {
        restartLevelButton->OnClicked -= playClickHandle;
    }
    if (quitToMainMenuButton != nullptr && quitClickHandle >= 0)
    {
        quitToMainMenuButton->OnClicked -= quitClickHandle;
    }
}

void GameController::Update()
{
    if (!mouseWasPressed && mouse->LeftPressed() && popup != nullptr)
    {
        popup->NextMessage();
    }
    // Freeze physics
    GetService<PhysicsWorld>()->SetFrozen(popup->IsShown() || isGameOver);
    mouseWasPressed = mouse->LeftPressed();
    if (doRestart)
    {
        HideGameOver();
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
        SceneWorld->LoadSafe(SceneWorld->GetFilePath());
        doRestart = false;
    }
}

void GameController::GameOver(bool win)
{
    isGameOver = true;
    gameOverText->text = win ? "Your ghost friends got you to safety!" : "Game Over! You were spotted.";
    gameOverText->layout.mainColor = win ? Ossium::Colors::GREEN : Ossium::Colors::RED;
    gameOverText->boxed = true;
    gameOverText->dirty = true;
    if (gui != nullptr)
    {
        gui->SetActive(true);
        if (!win && restartLevelButton->sprite != nullptr)
        {
            restartLevelButton->sprite->SetRenderWidth(1);
            restartLevelButton->GetEntity()->GetComponentInChildren<Text>()->text = "Retry";
            restartLevelButton->GetEntity()->GetComponentInChildren<Text>()->dirty = true;
        }
        if (quitToMainMenuButton->sprite != nullptr)
        {
            quitToMainMenuButton->sprite->SetRenderWidth(1);
            quitToMainMenuButton->GetEntity()->GetComponentInChildren<Text>()->text = "Quit to title";
            quitToMainMenuButton->GetEntity()->GetComponentInChildren<Text>()->dirty = true;
        }
    }
}

void GameController::RestartLevel()
{
    HideGameOver();
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
    SceneWorld->LoadSafe(SceneWorld->GetFilePath());
    doRestart = true;
}

void GameController::HideGameOver()
{
    isGameOver = false;
    gameOverText->text = "";
    gameOverText->boxed = false;
    gameOverText->dirty = true;
    if (gui != nullptr)
    {
        gui->SetActive(false);
        if (restartLevelButton->sprite != nullptr)
        {
            restartLevelButton->sprite->SetRenderWidth(0);
            restartLevelButton->GetEntity()->GetComponentInChildren<Text>()->text = "";
            restartLevelButton->GetEntity()->GetComponentInChildren<Text>()->dirty = true;
        }
        if (quitToMainMenuButton->sprite != nullptr)
        {
            quitToMainMenuButton->sprite->SetRenderWidth(0);
            quitToMainMenuButton->GetEntity()->GetComponentInChildren<Text>()->text = "";
            quitToMainMenuButton->GetEntity()->GetComponentInChildren<Text>()->dirty = true;
        }
    }
}

void GameController::QuitToMainMenu()
{
    HideGameOver();
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
    SceneWorld->ClearSafe();
    if (SceneMenu == nullptr)
    {
        SceneMenu = GetService<ResourceController>()->Get<Scene>("assets/scenes/MainMenu.rawr", entity->GetScene()->GetServices());
    }
    SceneUI->ClearSafe();
    SceneMenu->LoadSafe("assets/scenes/MainMenu.rawr");
}
