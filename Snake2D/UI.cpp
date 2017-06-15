#include "UI.h"
#include "App.h"
#include "AppInput.h"
#include "GameEntity.h"
#include "UIComponent.h"

Sprite UI::cursor;
FBounds UI::cursorBounds;
bool UI::cursorEnabled = true;

void UI::Update()
{
	auto& input = App::Instance().input;

	if (cursor.bitmap || !cursorEnabled)
	{
		auto mpos = input->GetMousePosition();
		auto bmpSize = cursor.bitmap->GetSize();

		cursorBounds = FBounds(FRect(mpos.x, mpos.y, bmpSize.width + mpos.x, bmpSize.height + mpos.y));
	}
	else
	{
		auto mpos = input->GetMousePosition();
		cursorBounds = FBounds(FRect(mpos.x, mpos.y, mpos.x+1, mpos.y+1));
	}

}

void UI::Render()
{
	if (!cursor.bitmap || !cursorEnabled)
	{
		return;
	}

	auto& gfx = App::Instance().gfx;

	auto& input = App::Instance().input;
	auto cursorSz = cursor.bitmap->GetSize();

	// Draw cursor bitmap
	gfx->DrawBitmap(
		cursor.bitmap,
		D2D1::RectF(input->GetMousePosition().x, input->GetMousePosition().y,
			cursorSz.width + input->GetMousePosition().x, cursorSz.height + input->GetMousePosition().y),
		1.F,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0, 0,
			cursor.bitmap->GetSize().width, // this transform the image and not renderer
			cursor.bitmap->GetSize().height));
}


bool UI::IsPointerOverUIElement(UIComponent& component)
{
	return component.tBounds.Interesects(cursorBounds);
}

bool UI::IsPointerOverUIElement(weakPtr<UIComponent> component)
{
	return component->tBounds.Interesects(cursorBounds);
}
