#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <Ossium.h>

using namespace Ossium;

class Popup;

struct GameControllerSchema : public Schema<GameControllerSchema, 20>
{
    DECLARE_BASE_SCHEMA(GameControllerSchema, 20);

    // The next level to load.
    M(std::string, nextLevel) = "";

};

class GameController : public Component, public GameControllerSchema
{
public:
    CONSTRUCT_SCHEMA(Component, GameControllerSchema);
    DECLARE_COMPONENT(Component, GameController);

    void OnLoadFinish();

    void OnDestroy();

    void Update();

    void GameOver();

    void RestartLevel();

    void QuitToMainMenu();

    Popup* popup = nullptr;

private:
    void CleanupHandles();

    void HideGameOver();

    bool isGameOver = false;

    bool mouseWasPressed = false;

    int playClickHandle = -1;
    int quitClickHandle = -1;

    Scene* worldScene = nullptr;

    Text* gameOverText = nullptr;
    Button* restartLevelButton = nullptr;
    Button* quitToMainMenuButton = nullptr;
    InputGUI* gui = nullptr;

    InputContext popupContext;

    MouseHandler* mouse = nullptr;

};


#endif // GAMECONTROLLER_H
