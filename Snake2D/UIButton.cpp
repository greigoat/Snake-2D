#include "UIButton.h"
#include "UI.h"
#include "App.h"
#include "AppInput.h"
#include "GameEntity.h"
#include <iostream>

Sprite UIButton::GetSprite()
{
	return image->sourceImage;
}

void UIButton::Tick()
{
	if (UI::IsPointerOverUIElement(*image.ptr.lock()))
	{
		if (image != nullptr)
		{
			image->color = highlightedColor;
		}

		if (!mouseOver)
		{
			onMouseEnter.Invoke();
			mouseOver = true;
		}

		//image.lock()->m_useColor = true;
		//image.lock()->m_color = highlightedColor;

		onMouseOver.Invoke();

		if (App::Instance().input->GetMouseButtonDown(VK_LBUTTON))
		{
			onClick.Invoke();
		}
	}
	else
	{
		// TODO: This is hack eg might not work as expected
		if (image != nullptr)
		{
			image->color = highlightedColor;
		}

		if (mouseOver)
		{
			onMouseExit.Invoke();
			mouseOver = false;
		}

		//image.lock()->m_useColor = false;
	}
}

void UIButton::BeginLoad()
{
	image = gameEntity->FindComponent<UIImage>();
}

void UIButton::Destroy()
{
}

void UIButton::Render()
{
}
