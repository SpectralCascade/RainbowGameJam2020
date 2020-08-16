#ifndef MAINMENU_H
#define MAINMENU_H

#include <Ossium.h>

using namespace Ossium;

struct MainMenuSchema : public Schema<MainMenuSchema, 10>
{
    DECLARE_BASE_SCHEMA(MainMenuSchema, 10);

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

    /// Load the main game scene.
    void GoPlay();

    /// Quit the game.
    void Quit();

};

#endif // MAINMENU_H
