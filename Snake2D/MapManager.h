#pragma once

#include <vector>
#include <string>
#include "weakPtr.h"

class GameEntity;
class Map;

/// <summary> 
/// Represents map manager that handles map loading/swithing/unloading
/// </summary>
class MapManager
{
	private:

	MapManager();
	~MapManager();

	private:

	::std::vector<std::shared_ptr<Map>> loadedMaps;
	weakPtr<Map> activeMap;

	public:

	/// <summary> 
	/// Get map by name that is loaded
	/// </summary>
	weakPtr<Map> GetMap(const std::string& name);

	/// <summary> 
	/// Get currently active map
	/// </summary>
	weakPtr<Map> GetActiveMap();

	/// <summary> 
	/// Load map
	/// </summary>
	weakPtr<Map> LoadMap(const std::string& name);

	/// <summary> 
	/// Create new map
	/// </summary>
	weakPtr<Map> CreateMap(std::string name);
	
	/// <summary> 
	/// Find game entities of given type in this map
	/// </summary>
	template<typename T> 
	std::vector<weakPtr<GameEntity>> FindGameEntitiesOfType();

	/// <summary> 
	/// Find all components of given type in this map
	/// </summary>
	template<typename T> 
	std::vector<weakPtr<T>> FindComponentsOfType();

	/// <summary> 
	/// Set active map
	/// </summary>
	void SetActiveMap(weakPtr<Map> map);

	/// <summary> 
	/// Destroy active map
	/// </summary>
	void DestroyMap(weakPtr<Map> map);

	/// <summary> 
	/// Updates map manager
	/// </summary>
	void Update();

	/// <summary> 
	/// Render map manager levels
	/// </summary>
	bool Render();

	friend class App;
	friend struct std::default_delete<MapManager>;
	friend class std::allocator<MapManager>;
};

template<typename T>
inline std::vector<weakPtr<GameEntity>>
MapManager::FindGameEntitiesOfType()
{
	std::vector<weakPtr<GameEntity>> outEntities;

	if (activeMap != nullptr)
		for (auto& e : activeMap->gameEntities)
			for (auto& com : e->components)
				if (std::dynamic_pointer_cast<T>(com))
					outEntities.push_back(e);

	return outEntities;
}

template<typename T>
inline std::vector<weakPtr<T>>
MapManager::FindComponentsOfType()
{
	std::vector<weakPtr<T>> outComs;

	if (activeMap != nullptr)
		for (auto& e : activeMap->gameEntities)
			for (auto& com : e->components)
				if (auto& c = std::dynamic_pointer_cast<T>(com))
					outComs.push_back(c);

	return outComs;
}
