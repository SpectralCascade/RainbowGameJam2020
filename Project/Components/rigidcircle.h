#ifndef RIGIDCIRCLE_H
#define RIGIDCIRCLE_H

#include "collider.h"

struct RigidCircleSchema : public Schema<RigidCircleSchema, 1>
{
    DECLARE_BASE_SCHEMA(RigidCircleSchema, 1);

    M(float, radius) = 1.0f;

};

class RigidCircle : public Collider, public RigidCircleSchema
{
public:
    CONSTRUCT_SCHEMA(Collider, RigidCircleSchema);
    DECLARE_COMPONENT(Collider, RigidCircle);

    void OnLoadFinish();

private:
    b2CircleShape shape;

};


#endif // RIGIDCIRCLE_H
