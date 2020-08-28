#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <Ossium.h>

using namespace Ossium;

class Popup;

struct GameControllerSchema : public Schema<GameControllerSchema, 20>
{
    DECLARE_BASE_SCHEMA(GameControllerSchema, 20);

};

class GameController : public Component
{
public:
    CONSTRUCT_SCHEMA(Component, GameController);
    DECLARE_COMPONENT(Component, GameController);

    void OnLoadFinish();

    void Update();

    Popup* popup = nullptr;

};


#endif // GAMECONTROLLER_H
