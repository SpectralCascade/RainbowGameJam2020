#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <Ossium.h>

using namespace Ossium;

class GameController : public Component
{
public:
    DECLARE_COMPONENT(Component, GameController);

    void OnLoadFinish();

};


#endif // GAMECONTROLLER_H
