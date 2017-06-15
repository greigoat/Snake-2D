#include "AppInput.h"
#include <Windows.h>
#include "App.h"

AppInput::AppInput()
{
}

AppInput::~AppInput()
{
}

FVector2 AppInput::GetMousePosition() const
{

#ifdef WINDOWS_PLATFORM
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(App::Instance().coreWnd, &p);
	return FVector2((float)p.x, (float)p.y);
#else
	return Vector2F(); // NOT IMPL:
#endif // WINDOWS_PLATFORM

}

bool AppInput::GetKeyDown(UInt inKey)
{
	return keys[inKey].wasDown && !keys[inKey].isDown;
}

bool AppInput::GetKeyUp(UInt inKey)
{
	return !keys[inKey].wasDown && !keys[inKey].isUp;
}

bool AppInput::GetKey(UInt inKey)
{
	return keys[inKey].isDown;
}

bool AppInput::GetMouseButtonDown(UInt inButton)
{
	return keys[inButton].wasDown && !keys[inButton].isDown;
}

bool AppInput::GetMouseButtonUp(UInt inButton)
{
	return !keys[inButton].wasDown && !keys[inButton].isUp;
}

bool AppInput::GetMouseButton(UInt inButton)
{
	return keys[inButton].isDown;
}

void AppInput::Init()
{
	for (size_t k = 0; k < 256; k++)
	{
		keys[k].key = (char)k;
		keys[k].isUp = true;
	}
}

void AppInput::Read()
{
#if WINDOWS_PLATFORM

	for (size_t i = 0; i < 256; i++)
	{
		if ((GetAsyncKeyState(keys[i].key) & 0x8000))
		{
			if (!keys[i].wasDown)
				keys[i].wasDown = true;
			else
			{
				keys[i].isDown = true;
				keys[i].isUp = false;
			}
		}
		else if (GetAsyncKeyState(keys[i].key) == 0)
		{
			if (keys[i].wasDown)
				keys[i].wasDown = false;
			else
			{
				keys[i].isDown = false;
				keys[i].isUp = true;
			}
		}
	}
#endif

}
