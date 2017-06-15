#pragma once

#include "UIComponent.h"
#include "Sprite.h"
#include "FColor.h"
#include "UPixel2.h"
#include "FVector2.h"
#include "FBounds.h"

/// <summary> 
/// UIImage component.
/// Represents renderable ui image
/// </summary>
class UIImage : public UIComponent
{
	public:
	// color of this image
	FColor color;
	// sprite of this image
	Sprite sourceImage;

	private:

	ComPtr<ID2D1Effect> colorMatrixEffect;
	D2D1_MATRIX_5X4_F colorMatrix;

	public:

	Sprite GetSprite() override;
	void SetSprite(const Sprite& sprite);
	void Tick() override;
	void Render() override;
	void BeginLoad() override;
	void Destroy() override;
};