#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <Ossium.h>

using namespace Ossium;

class Popup;

struct GameControllerSchema : public Schema<GameControllerSchema, 20>
{
    DECLARE_BASE_SCHEMA(GameControllerSchema, 20);

};

class GameController : public Component, public GameControllerSchema
{
public:
    CONSTRUCT_SCHEMA(Component, GameControllerSchema);
    DECLARE_COMPONENT(Component, GameController);

    void OnCreate();

    void OnDestroy();

    void OnLoadFinish();

    void Update();

    Popup* popup = nullptr;

private:
    bool mouseWasPressed = false;

    InputContext gameInput;
    MouseHandler* mouse = nullptr;

};


#endif // GAMECONTROLLER_H
