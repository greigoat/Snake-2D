#include "Screen.h"

#include <Windows.h>
#include "App.h"

UPixel2 Screen::GetSize()
{
#ifdef WINDOWS_PLATFORM
	HMONITOR hm = MonitorFromWindow(HWND_DESKTOP, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mInfo{ sizeof(MONITORINFO) };
	mInfo.cbSize = sizeof(MONITORINFO);

	GetMonitorInfo(hm, &mInfo);

	UInt w = mInfo.rcMonitor.right - mInfo.rcMonitor.left;
	UInt h = mInfo.rcMonitor.bottom - mInfo.rcMonitor.top;

	return{ w,h };
#endif // WINDOWS_PLATFORM

	return{};
}
