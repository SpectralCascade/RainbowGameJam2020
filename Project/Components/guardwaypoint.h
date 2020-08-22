#ifndef GUARDWAYPOINT_H
#define GUARDWAYPOINT_H

#include <Ossium.h>

using namespace Ossium;
using namespace std;

struct GuardWaypointSchema : public Schema<GuardWaypointSchema, 2>
{
    DECLARE_BASE_SCHEMA(GuardWaypointSchema, 2);

    M(string, nextEntityName);

};

typedef
#ifdef OSSIUM_EDITOR
    GraphicComponent
#else
    Component
#endif // OSSIUM_EDITOR
EditorOnlyGraphic;

class GuardWaypoint : public EditorOnlyGraphic, public GuardWaypointSchema
{
public:
    CONSTRUCT_SCHEMA(EditorOnlyGraphic, GuardWaypointSchema);
    DECLARE_COMPONENT(EditorOnlyGraphic, GuardWaypoint);

    void OnLoadFinish();

    // Only called in editor
    void Render(Renderer& renderer);

    GuardWaypoint* GetNextWaypoint();

private:
    GuardWaypoint* next = nullptr;

#ifdef OSSIUM_EDITOR
    string oldNextIdent;
#endif // OSSIUM_EDITOR

};

#endif // GUARDWAYPOINT_H
