#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include "Component.h"
#include "FTransform2D.h"
#include "MapManager.h"
#include "Map.h"
#include "App.h"

/// <summary> 
/// Represents an entity in a scene
/// </summary>
class GameEntity
{
	public:

	/// <summary> 
	/// is game entity active? Inactive objects are not rendered.
	/// </summary>
	bool active;

	GameEntity(const std::string& inName);
	~GameEntity();

	private:
	
	// The blob of components of this object
	std::vector<std::shared_ptr<Component>> components;

	public:

	/// <summary> 
	/// The name of this game entity
	/// </summary>
	std::string name;

	/// <summary> 
	/// The transform of this game entity
	/// </summary>
	weakPtr<FTransform2D> transform;

	public:

	/// <summary> 
	/// This is called per frame
	/// </summary>
	void Tick();

	/// <summary> 
	/// This is called on rendering pipleline
	/// </summary>
	void Render();

	/// <summary> 
	/// Use this to destroy game entity
	/// </summary>
	void Destroy();

	/// <summary> 
	/// Find component on this game object by given type
	/// </summary>
	template<typename T> weakPtr<T> FindComponent();

	/// <summary> 
	/// Add component to this game object by given type
	/// </summary>
	template<typename T> weakPtr<T> AttachComponent();

	/// <summary> 
	/// Remove component(s) from this game object by given type
	/// </summary>
	template<typename T> void DetachComponent();

	friend class MapManager;
};


template<typename T>
inline weakPtr<T> GameEntity::FindComponent()
{
	for (auto& com : components)
		if (dynamic_cast<T*>(com.get()))
			return std::dynamic_pointer_cast<T>(com);

	return std::shared_ptr<T>(nullptr);
}


template<typename T>
inline weakPtr<T> GameEntity::AttachComponent()
{
	auto com = std::shared_ptr<T>(new T);
	com->gameEntity = this;
	components.push_back(com);
	components.back()->BeginLoad();

	return std::dynamic_pointer_cast<T>(components.back());
}


template<typename T>
inline void GameEntity::DetachComponent()
{
	auto it = std::find_if(
		components.begin(), 
		components.end(),
		[&] (const std::shared_ptr<IComponent>& entity)
	{
		if (std::dynamic_pointer_cast<T*>(entity))
			return true;

		return false;
	});

	if (it != components.end())
	{
		(*it)->Destroy();
		it->reset();
		components.erase(it);
	}
}