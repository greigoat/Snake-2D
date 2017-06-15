#pragma once

#include "Component.h"
#include "Sprite.h"
#include "Component.h"
#include "FBounds.h"

/// <summary> 
/// UIComponent.
/// Base component class of all gui elements
/// </summary>
struct UIComponent : public RendererComponent
{
	// transformed bounds
	FBounds tBounds;

	/// <summary> 
	/// Get sprite of this ui component
	/// </summary>
	virtual Sprite GetSprite() = 0;
};