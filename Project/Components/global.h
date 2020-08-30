#ifndef GLOBAL_H
#define GLOBAL_H

class Popup;

namespace Ossium
{

    class Scene;

}

extern Ossium::Scene* SceneMenu;

extern Ossium::Scene* SceneUI;

extern Ossium::Scene* SceneWorld;

extern int totalGhosts;

extern bool EnteredHub;
extern bool EnteredLevel2;
extern bool EnteredLevel3;
extern bool EnteredExit;

Popup* FindPopup();

#endif // GLOBAL_H
