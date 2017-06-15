#include "App.h"
#include <Windows.h>
#include "Screen.h"
#include <iostream>

int main()
{
	// Initialize com
	CoInitializeEx(0, 0);
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	// Create application singleton
	App &app = App::Instance();

	// Initialzie app
	app.Initialize();

	// Describe app window params
	AppCoreWindowParams params{};
	params.hardwareCursorEnabled = false;
	params.resolution.x = 1024;
	params.resolution.y = 768;
	params.titlebarName = "Snake2D";
	params.position.x = (Screen::GetSize().x / 2) - (params.resolution.x / 2);
	params.position.y = (Screen::GetSize().y / 2) - (params.resolution.y / 2);
	params.windowStyle = WS_POPUP;

	// Create core window
	app.CreateCoreWindow(std::move(params));

	// Load application
	app.OnLoad();
	// Run app
	app.Run();
	// suspend app
	app.OnSuspend();

	return 0;
}

