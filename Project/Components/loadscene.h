#ifndef LOADSCENE_H
#define LOADSCENE_H

#include <Ossium.h>

using namespace Ossium;

class Player;

struct LoadSceneSchema : public Schema<LoadSceneSchema, 5>
{
    DECLARE_BASE_SCHEMA(LoadSceneSchema, 5);

    M(std::string, sceneToLoad) = "";

};

class LoadScene : public Component, public LoadSceneSchema
{
public:
    CONSTRUCT_SCHEMA(Component, LoadSceneSchema);
    DECLARE_COMPONENT(Component, LoadScene);

    void OnLoadFinish();

    void Update();

private:
    PhysicsBody* playerCollider = nullptr;

    PhysicsBody* sensor = nullptr;

};

#endif // LOADSCENE_H
