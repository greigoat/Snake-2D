#pragma once

/// <summary> 
/// Represents argb color
/// </summary>
struct FColor
{
	FColor() : FColor(1.0F, 1.0F, 1.0F, 1.0F)
	{
	}

	FColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
	{
	}

	static FColor White()
	{
		return FColor(1, 1, 1, 1);
	}

	float r, g, b, a;
};