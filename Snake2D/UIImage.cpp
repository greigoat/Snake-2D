#include "UIImage.h"
#include "App.h"
#include <vector>
#include "GameEntity.h"

Sprite UIImage::GetSprite()
{
	return sourceImage;
}

void UIImage::SetSprite(const Sprite & sprite)
{
	sourceImage = sprite;

	// Create effect and set its input to related bitmap
	auto& gfx = App::Instance().gfx;

	gfx->d2dImmediateContext->CreateEffect(CLSID_D2D1ColorMatrix, &colorMatrixEffect);

	colorMatrixEffect->SetInput(0, sourceImage.bitmap);
}

void UIImage::Tick()
{
	// Do not update if bitmap is not set
	if (!sourceImage.bitmap)
		return;

	// Define matrix
	colorMatrix = D2D1::Matrix5x4F(
		color.r, 0, 0, 0, // R
		0, color.g, 0, 0, // G
		0, 0, color.b, 0, // B
		0, 0, 0, 0, // what?
		0, 0, 0, color.a // alpha
	);

	std::vector<FVector2> verts;
	verts.push_back(FVector2(bounds.rect.left, bounds.rect.top));
	verts.push_back(FVector2(bounds.rect.right, bounds.rect.top));
	verts.push_back(FVector2(bounds.rect.right, bounds.rect.bottom));
	verts.push_back(FVector2(bounds.rect.left, bounds.rect.bottom));

	gameEntity->transform->matrix.Transform(verts);

	tBounds = FBounds(FRect(verts[0].x, verts[0].y, 
		verts[1].x + sourceImage.bitmap->GetSize().width, 
		verts[1].y + sourceImage.bitmap->GetSize().height));

	// Set color our colorMatrixEffect value
	colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
}

void UIImage::Render()
{
	// Do not render if bitmap is not set
	if (!sourceImage.bitmap)
		return;

	// Get gfx handler
	auto& gfx = App::Instance().gfx;
	auto bmpsz = sourceImage.bitmap->GetSize();

	auto rect = D2D1::RectF(0, 0, bmpsz.width, bmpsz.height);

	// Draw final image on the screen
	gfx->d2dImmediateContext->DrawImage(colorMatrixEffect, D2D1::Point2F(tBounds.rect.left, tBounds.rect.top), 
		rect, D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC);
}

void UIImage::BeginLoad()
{
	color = FColor();
}

void UIImage::Destroy()
{
}