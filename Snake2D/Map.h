#pragma once

#include <vector>
#include <string>
#include <memory>
#include "weakPtr.h"

class GameEntity;

/// <summary> 
/// Represents an level/scene that can be loaded/unloaded
/// </summary>
class Map
{
	public:

	/// <summary> 
	/// name of this scene
	/// </summary>
	std::string name;

	private:

	std::vector<std::shared_ptr<GameEntity>> gameEntities;

	public:

	/// <summary> 
	/// get count of root objects in this scene
	/// </summary>
	size_t GetRootCount();

	/// <summary> 
	/// Add new entity to this scene
	/// </summary>
	weakPtr<GameEntity> AddEntity(const std::string& inName);

	/// <summary> 
	/// Destroy entity from this scene
	/// </summary>
	void DestroyEntity(weakPtr<GameEntity> gameEntity);

	/// <summary> 
	/// Update map
	/// </summary>
	void Update();

	/// <summary> 
	/// Render map content
	/// </summary>
	bool Render();

	Map(const std::string & name);
	~Map();

	private:

	friend class MapManager;
	friend struct std::default_delete<Map>;
	friend class std::allocator<Map>;
	friend class GameEntity;
};