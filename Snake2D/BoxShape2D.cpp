#include "BoxShape2D.h"
#include "AppGfx.h"
#include "App.h"

#include <iostream>
#include "GameEntity.h"
#include "FMatrix3x3.h"

BoxShape2D::BoxShape2D()
{
	size = FVector2::One();
}

BoxShape2D::~BoxShape2D()
{
}

void BoxShape2D::BeginLoad()
{
	bounds = FBounds(FRect(0, 0, size.x, size.y));
}

void BoxShape2D::Tick()
{
	// Update bounds according to transform matrix
	std::vector<FVector2> verts;
	verts.push_back(FVector2(bounds.rect.left, bounds.rect.top));
	verts.push_back(FVector2(bounds.rect.right, bounds.rect.top));
	verts.push_back(FVector2(bounds.rect.right, bounds.rect.bottom));
	verts.push_back(FVector2(bounds.rect.left, bounds.rect.bottom));

	gameEntity->transform->matrix.Transform(verts);

	transformedBounds = FBounds(FRect(verts[0].x, verts[0].y, verts[1].x + size.x, verts[1].y + size.y));
}

void BoxShape2D::Destroy()
{
}

void BoxShape2D::Render()
{
	auto rc = transformedBounds.rect;
	App::Instance().gfx->FillRect(rc, FColor::White());
}

void BoxShape2D::SetSize(FVector2 size)
{
	this->size = size;
}
