#pragma once

#include "UIImage.h"
#include "UIEvent.h"
#include "weakPtr.h"

/// <summary> 
/// UI Button component.
/// Represents clickable UI button
/// </summary>
class UIButton : public UIComponent
{
	public:

	// the image of the button
	weakPtr<UIImage> image;
	// fired when user clicks on the button
	UIEvent onClick;
	// fired when mouse is over this button
	UIEvent onMouseOver;
	// fired when mouse moved from this button
	UIEvent onMouseExit;
	// fired when mouse just entered this button
	UIEvent onMouseEnter;
	// the highlighted color of this button
	FColor highlightedColor;

	// get sprite of this button
	Sprite GetSprite() override;
	void Tick() override;
	void BeginLoad() override;
	void Destroy() override;
	void Render() override;

	private:

	bool mouseOver;

};