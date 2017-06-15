#pragma once

#include "UIEvent.h"
#include "UIImage.h"
#include "UIButton.h"

/// <summary> 
/// Represents UI manager class
/// </summary>
class UI
{
	public:

	// the cursor that is loaded
	static Sprite cursor;
	// the bounds of cursor
	static FBounds cursorBounds;
	// is cursor enabled?
	static bool cursorEnabled;

	private:

	public:

	// Update ui manager
	static void Update();
	// Rendere ui manager
	static void Render();

	// Is pointer currently over an gui element
	static bool IsPointerOverUIElement(UIComponent& component);
	static bool IsPointerOverUIElement(weakPtr<UIComponent> component);
};