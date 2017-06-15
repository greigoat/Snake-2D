#include "GameEntity.h"

GameEntity::GameEntity(const std::string & inName) : name(inName) 
{
	active = true;
	transform = AttachComponent<FTransform2D>();
}

GameEntity::~GameEntity()
{
}

void GameEntity::Tick()
{
	if (active)
	{
		for (auto& c : components)
			c->Tick();
	}
}


void GameEntity::Render()
{
	if (active)
	{
		for (auto& c : components)
			if (auto &com = std::dynamic_pointer_cast<IRendererComponent>(c))
				com->Render();
	}
}


void GameEntity::Destroy()
{
	for (auto& c : components)
	{
		c->Destroy();
		c.reset();
	}
}
