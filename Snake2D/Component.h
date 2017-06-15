#pragma once

#include <string>
#include "FBounds.h"

class GameEntity;

/// <summary> 
/// IComponent interface.
/// Represents interfrace of an game object component.
/// </summary>
struct IComponent
{
	virtual void BeginLoad() = 0;
	virtual void Tick() = 0;
	virtual void Destroy() = 0;
};

/// <summary> 
/// Component.
/// Base class of all components.
/// </summary>
class Component : public virtual IComponent
{
	public:

	Component();
	~Component();

	public:

	/// <summary> 
	/// The name of this object.
	/// </summary>
	std::string name;

	// TODO: Write guid system
	/// <summary> 
	/// The guid of this object
	/// </summary>
	std::string guid;

	/// <summary> 
	/// The gameEntity the component is attached to. This cannot be null.
	/// </summary>
	GameEntity* gameEntity;

	private:
};

/// <summary> 
/// IRendererComponent.
/// Represents an interface for a RendererComponent.
/// </summary>
struct IRendererComponent : public IComponent
{
	/// <summary> 
	/// Draws content of this renderable component
	/// </summary>
	virtual void Render() = 0;
};

/// <summary> 
/// RendererComponent.
/// Represents base class of all components that can be rendererd by Gfx.
/// </summary>
class RendererComponent : public Component, public IRendererComponent
{
	public:

	/// <summary> 
	/// Bounds/vertices of an renderable component
	/// </summary>
	FBounds bounds;
};