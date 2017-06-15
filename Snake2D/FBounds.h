#pragma once

#include "FVector2.h"

/// <summary> 
/// Represents rectangle
/// </summary>
struct FRect
{
	FRect() : FRect(0, 0, 0, 0) {}

	FRect(float left, float top, float right, float bottom)
		: top(top), left(left), right(right), bottom(bottom)
	{
	}

	float top, left, right, bottom;
};

/// <summary> 
/// Represents bounds of an rectangle
/// </summary>
class FBounds
{
	public:

	FBounds() : FBounds(FRect()) {}
	FBounds(FRect rect) : rect(rect) {}

	FRect rect;

	FVector2 Min();
	FVector2 Max();

	bool Interesects(FBounds& b);

	private:
};