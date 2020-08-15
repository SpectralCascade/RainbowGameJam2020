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

class FitScreen : public GraphicComponent, public FitScreenSchema
{
public:
    CONSTRUCT_SCHEMA(GraphicComponent, FitScreenSchema);
    DECLARE_COMPONENT(GraphicComponent, FitScreen);

    void OnLoadFinish();

    void Render(Renderer& renderer);

private:
    bool dirty = true;

};

#endif // FITSCREEN_H
