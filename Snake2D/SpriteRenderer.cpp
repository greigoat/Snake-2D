#include "SpriteRenderer.h"
#include <vector>
#include "GameEntity.h"

SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::BeginLoad()
{
	if (sprite.HasBmp())
	{
		bounds = FBounds(FRect(0, 0, sprite.GetSize().x, sprite.GetSize().y));
	}
	else
	{
		bounds = FBounds(FRect(0, 0, 0, 0));
	}
}

void SpriteRenderer::Tick()
{
	// Update bounds according to transform matrix
	std::vector<FVector2> verts;
	verts.push_back(FVector2(bounds.rect.left, bounds.rect.top));
	verts.push_back(FVector2(bounds.rect.right, bounds.rect.top));
	verts.push_back(FVector2(bounds.rect.right, bounds.rect.bottom));
	verts.push_back(FVector2(bounds.rect.left, bounds.rect.bottom));

	gameEntity->transform->matrix.Transform(verts);

	tBounds = FBounds(FRect(verts[0].x, verts[0].y, verts[1].x + sprite.GetSize().x + spriteSizeFactor.x, verts[1].y + sprite.GetSize().y + spriteSizeFactor.y));
}

void SpriteRenderer::Destroy()
{
}

void SpriteRenderer::Render()
{
	auto rc = tBounds.rect;
	auto dstRect = D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom);

	if (sprite.bitmap)
	{
		App::Instance().gfx->DrawBitmap(sprite.bitmap, dstRect, 1, 
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0,0,sprite.GetSize().x, sprite.GetSize().y));
	}
}
