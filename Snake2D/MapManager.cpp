#include "App.h"
#include "MapManager.h"
#include "Map.h"

#include <algorithm>

MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}

void MapManager::SetActiveMap(weakPtr<Map> map)
{
	activeMap = map;
}

weakPtr<Map> MapManager::GetMap(const std::string& name)
{
	for (size_t i = 0; i < loadedMaps.size(); i++)
	{
		if (loadedMaps[i]->name == name)
		{
			return loadedMaps[i];
		}
	}

	return std::shared_ptr<Map>(nullptr);
}

weakPtr<Map> MapManager::GetActiveMap()
{
	return activeMap;
}

void MapManager::Update()
{
	if (activeMap != nullptr)
		activeMap->Update();
}

bool MapManager::Render()
{
	if (!activeMap)
		return false;

	activeMap->Render();

	return true;
}

weakPtr<Map> MapManager::LoadMap(const std::string& name)
{
	{
		weakPtr<Map> map = GetMap(name);

		if (map != nullptr)
			return map;
	}

	// TODO: !--NOT IMPLEMENTED

	return loadedMaps.back();
}

weakPtr<Map> MapManager::CreateMap(std::string name)
{
	{
		weakPtr<Map> map = GetMap(name);

		if (map != nullptr)
			return map;
	}

	loadedMaps.push_back(std::shared_ptr<Map>(new Map(name)));

	return loadedMaps.back();
}

void MapManager::DestroyMap(weakPtr<Map> map)
{
	if (!map)
		return;

	auto it = std::find_if(
		loadedMaps.begin(),
		loadedMaps.end(),
		[&] (const std::shared_ptr<Map>& mapElem)
	{
		return mapElem->name == map->name;
	});

	if (it != loadedMaps.end())
	{
		it->reset();
		loadedMaps.erase(it);
	}
}