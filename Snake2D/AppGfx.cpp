#include "AppGfx.h"
#include "App.h"
#include <dwrite.h>

AppGfx::~AppGfx()
{
	format->Release();
}

void AppGfx::FillRect(FRect rect, FColor color)
{
	m_d2dDefaultBrush->SetColor(D2D1::ColorF(color.r, color.b, color.b, color.a));
	d2dImmediateContext->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), m_d2dDefaultBrush);
}
//ComPtr<IDWriteTextFormat> format;
void AppGfx::Init()
{
#ifdef WINDOWS_PLATFORM

	HRESULT hr = S_OK;

	// Create ID2D1Factory
	ComPtr<ID2D1Factory> factory;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	factory->QueryInterface<ID2D1Factory2>(&m_d2dFactory);

	ComPtr<IDWriteFactory> writeFactory;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	writeFactory->CreateTextFormat(
		L"Gabriola",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"en-us",
		&format
	);

	// This flag adds support for surfaces with a different color channel ordering than the API default.
	// You need it for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// This array defines the set of DirectX hardware feature levels this app  supports.
			// The ordering is important and you should  preserve it.
			// Don't forget to declare your app's minimum required feature level in its
			// description.  All apps are assumed to support 9.1 unless otherwise stated.
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			// Create the DX11 API device object, and get a corresponding context.
			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11DeviceContext> context;

			D3D11CreateDevice(
				nullptr,                    // specify null to use the default adapter
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				creationFlags,              // optionally set debug and Direct2D compatibility flags
				featureLevels,              // list of feature levels this app can support
				ARRAYSIZE(featureLevels),   // number of possible feature levels
				D3D11_SDK_VERSION,
				&device,                    // returns the Direct3D device created
				&m_d3dFeatureLevel,            // returns feature level of device created
				&context                    // returns the device immediate context
			);

			// Obtain the underlying DXGI device of the Direct3D11 device.
			ComPtr<IDXGIDevice1> dxgiDevice;
			device.QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice);

			// Obtain the Direct2D device for 2-D rendering.
			m_d2dFactory->CreateDevice(dxgiDevice, &m_d2dDevice);


			// Get Direct2D device's corresponding device context object.
			m_d2dDevice->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				&d2dImmediateContext
			);

			// Allocate a descriptor.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = 0;                           // use automatic sizing
			swapChainDesc.Height = 0;
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
			swapChainDesc.Scaling = DXGI_SCALING_NONE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
			swapChainDesc.Flags = 0;
			
			// Identify the physical adapter (GPU or card) this device is runs on.
			ComPtr<IDXGIAdapter> dxgiAdapter;
			dxgiDevice->GetAdapter(&dxgiAdapter);

			// Identify the physical adapter (GPU or card) this device is runs on.
			ComPtr<IDXGIFactory2> dxgiFactory;
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

			// Get the factory object that created the DXGI device.
			ComPtr<IDXGISwapChain1> swapChain;
			dxgiFactory->CreateSwapChainForHwnd(
				device,
				App::Instance().coreWnd,
				&swapChainDesc,
				nullptr, // TODO: Implement full screen swap chain desc for full screen supprt
				nullptr,
				&swapChain
			);

			swapChain->QueryInterface<IDXGISwapChain2>(&m_dxgiSwapChain);

			// Ensure that DXGI doesn't queue more than one frame at a time.
			dxgiDevice->SetMaximumFrameLatency(1);
			
			// Get the backbuffer for this window which is be the final 3D render target.
			ComPtr<ID3D11Texture2D> backBuffer;
			m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

			// Now we set up the Direct2D render target bitmap linked to the swapchain. 
			// Whenever we render to this bitmap, it is directly rendered to the 
			// swap chain associated with the window.
			D2D1_BITMAP_PROPERTIES1 bitmapProperties =
				D2D1::BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
					96.0F, // May also use factory->GetDestkopDpi()
					96.0F
				);

			// Direct2D needs the dxgi version of the backbuffer surface pointer.
			ComPtr<IDXGISurface> dxgiBackBuffer;
			m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

			// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
			d2dImmediateContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer,
				&bitmapProperties,
				&m_d2dTargetBitmap
			);

			// Now we can set the Direct2D render target.
			d2dImmediateContext->SetTarget(m_d2dTargetBitmap);

			d2dImmediateContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_d2dDefaultBrush);
			d2dImmediateContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &brush);
#else
#endif
}

void AppGfx::BeginDraw()
{
#ifdef WINDOWS_PLATFORM
	d2dImmediateContext->BeginDraw();
#endif // WINDOWS_PLATFORM

}

void AppGfx::Clear()
{
#ifdef WINDOWS_PLATFORM
	d2dImmediateContext->Clear(
		D2D1::ColorF(
			m_clearColor.r,
			m_clearColor.g,
			m_clearColor.b,
			m_clearColor.a
		)
	);
#endif // WINDOWS_PLATFORM
}

void AppGfx::EndDraw()
{
#ifdef WINDOWS_PLATFORM
	d2dImmediateContext->EndDraw();
	
#endif // WINDOWS_PLATFORM
}

void AppGfx::Present(UInt syncInterval, UInt presentFlags)
{
#ifdef WINDOWS_PLATFORM
	m_dxgiSwapChain->Present1(syncInterval, presentFlags, &m_dxgiPresentParameters);
#endif // WINDOWS_PLATFORM
}

FVector2 AppGfx::GetRenderArea()
{
	return FVector2(d2dImmediateContext->GetSize().width, d2dImmediateContext->GetSize().height);
}

void AppGfx::DrawBitmap(
	ID2D1Bitmap * bitmap, 
	const D2D1_RECT_F & destinationRectangle, 
	float opacity, 
	D2D1_BITMAP_INTERPOLATION_MODE interpolationMode, 
	const D2D1_RECT_F & sourceRectangle)
{
	d2dImmediateContext->DrawBitmap(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle);
}
