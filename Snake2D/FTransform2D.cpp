#include "FTransform2D.h"
#include "GameEntity.h"
#include "UI.h"


void FTransform2D::BeginLoad()
{
	//location = FVector2::Zero();
	//theta = 0.0F;
	//scale = FVector2::One();
}

void FTransform2D::Tick()
{
	// 1. Find image on the game object.
	// 2. Recalculate bounds according to the image size.
}

void FTransform2D::Destroy()
{
}

void FTransform2D::SetLocation(const FVector2 & location)
{
	matrix.m[2][0] = location.x;
	matrix.m[2][1] = location.y;
}

FVector2 FTransform2D::GetLocation()
{
	return FVector2(matrix.m[2][0], matrix.m[2][1]);
}

float FTransform2D::GetTheta()
{
	return 0;
}

FVector2 FTransform2D::GetScale()
{
	return FVector2();
}

void FTransform2D::Translate(const FVector2 & translation)
{
	matrix.Translate(translation);
}

// Dat is lastig..LASTIG
void FTransform2D::Rotate(float theta)
{
	// TODO: ->
	// 1. Save current matrix translation
	// 2. Set transform matrix to identity
	// 3. Apply vertex transformation
	// 4. Calculate rotation
	// 5. Set matrix translation back
	// 6  Recalculate vertices

	FVector2 location = GetLocation();
	matrix.SetIdentity();
	matrix.Rotate(theta);
	SetLocation(location);
}

void FTransform2D::Scale(const FVector2 & scale)
{
	// TODO: ->
	// 1. Save current matrix translation
	// 2. Set transform matrix to identity
	// 3. Apply vertex transformation
	// 4. Calculate scale
	// 5. Set matrix translation back
	// 6  Recalculate vertices

	matrix.Scale(scale);
}
