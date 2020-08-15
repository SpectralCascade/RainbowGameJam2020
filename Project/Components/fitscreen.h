#ifndef FITSCREEN_H
#define FITSCREEN_H

#include <Ossium.h>

using namespace Ossium;

struct FitScreenSchema : public Schema<FitScreenSchema, 20>
{
    DECLARE_BASE_SCHEMA(FitScreenSchema, 20);

    M(Vector2, topLeftAnchor) = Vector2(0.0f, 0.0f);
    M(Vector2, bottomRightAnchor) = Vector2(1.0f, 1.0f);
    M(Vector2, transformOrigin) = Vector2(0.5f, 0.5f);
#ifdef OSSIUM_EDITOR
    M(bool, showDebug) = false;
#endif // OSSIUM_EDITOR

};


class FitScreen : public
#ifdef OSSIUM_EDITOR
GraphicComponent
#else
Component
#endif // OSSIUM_EDITOR
, public FitScreenSchema
{
public:
#ifdef OSSIUM_EDITOR
    CONSTRUCT_SCHEMA(GraphicComponent, FitScreenSchema);
    DECLARE_COMPONENT(GraphicComponent, FitScreen);
#else
    CONSTRUCT_SCHEMA(Component, FitScreenSchema);
    DECLARE_COMPONENT(Component, FitScreen);
#endif // OSSIUM_EDITOR

    void OnLoadFinish();

    void Render(Renderer& renderer);

private:
    bool dirty = true;

};

#endif // FITSCREEN_H
