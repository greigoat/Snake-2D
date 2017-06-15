#pragma once

#include <wincodec.h>

#include "AppGfx.h"

#include <string>

#include "UPixel2.h"
#include "FVector2.h"
#include "FBounds.h"

/// <summary> 
/// Reprents sprite/bitmap image that can be loaded and rendered by gfx
/// </summary>
class Sprite
{
	public:

	Sprite() {}
	Sprite(const std::wstring& filename);
	Sprite(wchar_t* filename);

	/// <summary> 
	/// Is bitmap loaded?
	/// </summary>
	bool HasBmp();

	/// <summary> 
	/// Loads bmp by path
	/// </summary>
	void LoadBmp(const std::wstring& filename);

	/// <summary> 
	/// Get pixel size of bitmap
	/// </summary>
	UPixel2 GetPixelSize();

	/// <summary> 
	/// Get unit size of bitmap
	/// </summary>
	FVector2 GetSize();

	private:

	ComPtr<ID2D1Bitmap1> bitmap;

	private:

	friend class UIImage;
	friend class UI;
	friend class SpriteRenderer;
};