#include "fitscreen.h"

using namespace Ossium;

REGISTER_COMPONENT(FitScreen);

void FitScreen::OnLoadFinish()
{
    GraphicComponent::OnLoadFinish();
    dirty = true;
}

void FitScreen::SetDirty()
{
    dirty = true;
}

void FitScreen::Render(Renderer& renderer)
{
    Vector2 screenDimensions = Vector2(renderer.GetViewportRect().w, renderer.GetViewportRect().h);
    if (oldRenderDimensions != screenDimensions)
    {
        oldRenderDimensions = screenDimensions;
        dirty = true;
    }
    if (dirty)
    {
        Transform* t = GetTransform();

        Vector2 bounds = (bottomRightAnchor * screenDimensions) - (topLeftAnchor * screenDimensions);
        t->SetLocalPosition((topLeftAnchor * screenDimensions) + (bounds * transformOrigin));

        auto texts = entity->GetComponents<Text>();
        for (Text* text : texts)
        {
            text->layout.SetBounds(bounds - t->GetLocalPosition());
            text->dirty = true;
        }
        dirty = false;
    }
#ifdef OSSIUM_EDITOR
    if (showDebug)
    {
        Vector2 pos = screenDimensions * topLeftAnchor;
        Rect(pos.x - 1, pos.y - 1, 3, 3).DrawFilled(renderer, Colors::RED);
        pos = screenDimensions * bottomRightAnchor;
        Rect(pos.x - 1, pos.y - 1, 3, 3).DrawFilled(renderer, Colors::RED);
        Rect(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y, 3, 3).DrawFilled(renderer, Colors::CYAN);
    }
#endif // OSSIUM_EDITOR
}
