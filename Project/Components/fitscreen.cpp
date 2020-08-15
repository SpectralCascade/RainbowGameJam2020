#include "fitscreen.h"

using namespace Ossium;

REGISTER_COMPONENT(FitScreen);

void FitScreen::OnLoadFinish()
{
#ifdef OSSIUM_EDITOR
    GraphicComponent::OnLoadFinish();
#else
    Component::OnLoadFinish();
#endif // OSSIUM_EDITOR
    dirty = true;
}

void FitScreen::Render(Renderer& renderer)
{
    if (dirty)
    {
        Transform* t = GetTransform();
        Vector2 screenDimensions = Vector2(renderer.GetViewportRect().w, renderer.GetViewportRect().h);

        t->SetLocalPosition((topLeftAnchor * screenDimensions) + (((bottomRightAnchor * screenDimensions) - (topLeftAnchor * screenDimensions)) * transformOrigin));
    }
#ifdef OSSIUM_EDITOR
    if (showDebug)
    {
        Vector2 screenDimensions = Vector2(renderer.GetViewportRect().w, renderer.GetViewportRect().h);
        Vector2 pos = screenDimensions * topLeftAnchor;
        Rect(pos.x, pos.y, 3, 3).DrawFilled(renderer, Colors::RED);
        pos = screenDimensions * bottomRightAnchor;
        Rect(pos.x, pos.y, 3, 3).DrawFilled(renderer, Colors::RED);
        Rect(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y, 3, 3).DrawFilled(renderer, Colors::CYAN);
    }
#endif // OSSIUM_EDITOR
}
