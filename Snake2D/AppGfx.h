#pragma once

#include <memory>
#include <comdef.h>
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1helper.h>

#include "FColor.h"
#include "intdef.h"

#include "FVector2.h"
#include "FBounds.h"

template<typename T>
using ComPtr = _com_ptr_t<_com_IIID<T, &__uuidof(T)>>;

/// <summary> 
/// Represents engine that renders graphics on the screen
/// </summary>
class AppGfx
{
	public:

	~AppGfx();

	ComPtr<ID2D1SolidColorBrush> brush;
	IDWriteTextFormat* format;
	//ComPtr<ID2D1Brush> textBrush;

	/// <summary> 
	/// Render rectangle by given dimensions
	/// </summary>
	void FillRect(FRect rect, FColor color);

	/// <summary> 
	/// Initializes graphics
	/// </summary>
	void Init();

	/// <summary> 
	/// Begin rendering
	/// </summary>
	void BeginDraw();

	/// <summary> 
	/// Clears the rendering area
	/// </summary>
	void Clear();

	/// <summary> 
	/// End rendering
	/// </summary>
	void EndDraw();

	/// <summary> 
	/// Present render target
	/// </summary>
	void Present(UInt syncInterval, UInt presentFlags);

	/// <summary> 
	/// Get renderable area
	/// </summary>
	FVector2 GetRenderArea();

	/// <summary> 
	/// Clear/Background color of the renderable area
	/// </summary>
	FColor m_clearColor = { 0, 0, 0, 1 };

	ComPtr<ID2D1SolidColorBrush> m_d2dDefaultBrush;
#ifdef WINDOWS_PLATFORM

	/// <summary> 
	/// Draws bitmap within renderable area
	/// </summary>
	void DrawBitmap(
		ID2D1Bitmap *bitmap,
		const D2D1_RECT_F &destinationRectangle,
		float opacity,
		D2D1_BITMAP_INTERPOLATION_MODE interpolationMode,
		const D2D1_RECT_F &sourceRectangle
	);

	// Private api tools, should not be exposed
	ComPtr<ID2D1DeviceContext1> d2dImmediateContext;

#endif // WINDOWS_PLATFORM


	private:

#ifdef WINDOWS_PLATFORM

	ComPtr<IDXGISwapChain2> m_dxgiSwapChain;
	ComPtr<ID2D1Factory2> m_d2dFactory;
	ComPtr<ID2D1Device1> m_d2dDevice;

	ComPtr<ID2D1Bitmap1> m_d2dTargetBitmap;
	DXGI_PRESENT_PARAMETERS m_dxgiPresentParameters;
	D3D_FEATURE_LEVEL m_d3dFeatureLevel;
#endif

	friend class Sprite; // For now :)
};