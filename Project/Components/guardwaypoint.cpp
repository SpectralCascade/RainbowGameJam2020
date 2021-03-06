#include "guardwaypoint.h"

REGISTER_COMPONENT(GuardWaypoint);

void GuardWaypoint::OnLoadFinish()
{
    ParentType::OnLoadFinish();
    // Build waypoint map
    if (nextEntityName.empty())
    {
        next = nullptr;
    }
    else if (next == nullptr
#ifdef OSSIUM_EDITOR
        || oldNextIdent != nextEntityName
#endif // OSSIUM_EDITOR
    ) {
        Entity* nextEnt = entity->Find(nextEntityName);
        if (nextEnt != nullptr)
        {
            next = nextEnt->GetComponent<GuardWaypoint>();
        }
        else
        {
            next = nullptr;
        }
    }
}

void GuardWaypoint::Render(Renderer& renderer)
{
    Rect(GetTransform()->GetWorldPosition().x - 3, GetTransform()->GetWorldPosition().y - 3, 7, 7).DrawFilled(renderer, Ossium::Colors::MAGENTA);
    if (next != nullptr)
    {
        // Check that next is actually valid before rendering... should probably just use a shared_ptr or something.
        Entity* located = entity->Find(nextEntityName);
        if (located != nullptr && located->GetComponent<GuardWaypoint>() != nullptr)
        {
            Line(GetTransform()->GetWorldPosition(), next->GetTransform()->GetWorldPosition()).Draw(renderer, Ossium::Colors::YELLOW);
        }
        else
        {
            next = nullptr;
        }
    }
}

GuardWaypoint* GuardWaypoint::GetNextWaypoint()
{
    return next;
}
