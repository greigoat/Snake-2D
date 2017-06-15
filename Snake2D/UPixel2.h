#pragma once

#include "intdef.h"

/// <summary> 
/// Represents 2D pixel
/// </summary>
struct UPixel2
{
	UPixel2() : UPixel2(0,0){}
	UPixel2(UInt x, UInt y) :x(x), y(y) {}

	UInt x, y;
};