#include "ghost.h"

REGISTER_COMPONENT(Ghost);

void Ghost::OnLoadFinish()
{
    sprite = entity->AddComponentOnce<StateSprite>();
}

void Ghost::Update()
{
}
