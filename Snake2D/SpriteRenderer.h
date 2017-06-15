#pragma once

#include "Component.h"
#include "FVector2.h"
#include "FBounds.h"
#include "Sprite.h"

/// <summary> 
/// SpriteRenderer comonent
/// Handles rendering of sprite
/// </summary>
class SpriteRenderer : public RendererComponent
{
	public:

	SpriteRenderer();
	~SpriteRenderer();

	public:
	
	FVector2 spriteSizeFactor;

	/// <summary> 
	/// The sprite that is loaded & attached to this component
	/// </summary>
	Sprite sprite;

	/// <summary> 
	/// transformed bounds of the sprite
	/// </summary>
	FBounds tBounds;

	public:

	virtual void BeginLoad() override;
	virtual void Tick() override;
	virtual void Destroy() override;
	virtual void Render() override;

	private:
};