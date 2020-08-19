#ifndef MAINMENU_H
#define MAINMENU_H

#include <Ossium.h>

using namespace Ossium;

struct MainMenuSchema : public Schema<MainMenuSchema, 10>
{
    DECLARE_BASE_SCHEMA(MainMenuSchema, 10);

    M(Vector2, ghostOffset) = { 0, 0 };
    M(float, speed) = 2.0f;

    Button* playButton = nullptr;
    Button* quitButton = nullptr;
    InputGUI* inputGUI = nullptr;

};

class MainMenu : public Component, public MainMenuSchema
{
public:
    CONSTRUCT_SCHEMA(Component, MainMenuSchema);
    DECLARE_COMPONENT(Component, MainMenu);

    /// Initialise buttons.
    void OnLoadFinish();

    /// Update animations.
    void Update();

    /// Load the main game scene.
    void GoPlay();

    /// Quit the game.
    void Quit();

private:
    void CleanupHandles();

    Vector2 ghostStartPos = Vector2::Zero;

    float animPercent = 0.25f;

    Delta delta;

    int playClickHandle = -1;
    int quitClickHandle = -1;

};

#endif // MAINMENU_H
