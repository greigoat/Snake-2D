#include "Sprite.h"
#include "App.h"


Sprite::Sprite(const std::wstring& filename)
{
	LoadBmp(filename);
}

Sprite::Sprite(wchar_t * filename)
{
	LoadBmp(filename);
}

bool Sprite::HasBmp()
{
	return bitmap != nullptr;
}

// TODO: Need to implement exception system or check and display errors 
void Sprite::LoadBmp(const std::wstring& filename)
{
	HRESULT hr = S_OK;

	// Create WIC Factory
	ComPtr<IWICImagingFactory> wicFactory;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory);

	if (SUCCEEDED(hr))
	{
		ComPtr<IWICBitmapDecoder> decoder;
		hr = wicFactory->CreateDecoderFromFilename(
			filename.c_str(), // The filename 
			nullptr, // vendor
			GENERIC_READ, // Read access
			WICDecodeMetadataCacheOnLoad, // Cache on load* 
			&decoder // decoder
		);

		if (SUCCEEDED(hr))
		{
			// Read a frame from image 
			ComPtr<IWICBitmapFrameDecode> wicFrame;
			hr = decoder->GetFrame(0, &wicFrame);

			if (SUCCEEDED(hr))
			{
				// Create format converter
				ComPtr<IWICFormatConverter> converter;
				hr = wicFactory->CreateFormatConverter(&converter);

				if (SUCCEEDED(hr))
				{
					hr = converter->Initialize(
						wicFrame,
						GUID_WICPixelFormat32bppPBGRA, // bitmap pixel format
						WICBitmapDitherTypeNone, // Optional bitmap dithering
						NULL, // palette
						0.0F, // Alpha transparency %
						WICBitmapPaletteTypeCustom // palette related stuff
					);

					if (SUCCEEDED(hr)) // Last one hopefully
					{
						App::Instance().gfx->d2dImmediateContext->CreateBitmapFromWicBitmap(
							converter,
							nullptr, // WIC_BITMAP_PROPS
							&bitmap
						);
					}
				}
			}
		}
	}
}

UPixel2 Sprite::GetPixelSize()
{
	return UPixel2(bitmap->GetPixelSize().width, bitmap->GetPixelSize().height);
}

FVector2 Sprite::GetSize()
{
	return FVector2(bitmap->GetSize().width, bitmap->GetSize().height);
}