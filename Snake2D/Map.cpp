#include "Map.h"
#include "GameEntity.h"
#include <algorithm>

size_t Map::GetRootCount()
{
	return gameEntities.size();
}

weakPtr<GameEntity> Map::AddEntity(const std::string & inName)
{
	gameEntities.push_back(std::shared_ptr<GameEntity>(new GameEntity(inName)));

	return gameEntities.back();
}

void Map::DestroyEntity(weakPtr<GameEntity> gameEntity)
{
	if (!gameEntity)
		return;

	auto it = std::find_if(gameEntities.begin(), gameEntities.end(), 
		[&] (const std::shared_ptr<GameEntity>& entity)
	{
		return entity->name == gameEntity->name;
	});

	if (it != gameEntities.end())
	{
		(*it)->Destroy();
		gameEntities.erase(it);
	}
}

void Map::Update()
{
	if (gameEntities.size() == 0)
		return;

	for (auto& entity : gameEntities)
	{
		entity->Tick();
	}
}

bool Map::Render()
{
	if (gameEntities.size() == 0)
		return false;

	for (auto& entity : gameEntities)
	{
		entity->Render();
	}

	return true;
}

Map::Map(const std::string& name)
	: name(name)
{
}

Map::~Map()
{
}