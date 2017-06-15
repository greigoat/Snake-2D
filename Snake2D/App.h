#pragma once

#include <string>
#include <memory>
#include <string>

#include "AppGfx.h"
#include "FColor.h"
#include "UPixel2.h"


#ifdef WINDOWS_PLATFORM
#include <Windows.h>
#endif // WINDOWS_PLATFORM

class SnakeGame;

class AppInput;
class MapManager;

/// <summary> 
/// Describes arguments that are passed in time of window creation.
/// </summary>
struct AppCoreWindowParams
{
	std::string titlebarName;
	UPixel2 resolution;
	UPixel2 position;
	bool hardwareCursorEnabled;

#ifdef WINDOWS_PLATFORM
	DWORD windowStyle;
#endif // WINDOWS_PLATFORM
};

/// <summary> 
/// Represents main application of the engine.
/// </summary>
class App
{
	public:

	App();
	~App();

	public:

	/// <summary> 
	/// Is application running?
	/// </summary>
	bool running;

	/// <summary> 
	/// The frame color of the core window.
	/// </summary>
	FColor wndFrameColor;

	/// <summary> 
	/// The name of an application. This will be displayed on the window titlebar.
	/// </summary>
	std::string name;

	/// <summary> 
	/// Input handler of this application.
	/// </summary>
	std::unique_ptr<AppInput> input;

	/// <summary> 
	/// Map manager handler of this application.
	/// </summary>
	std::unique_ptr<MapManager> mapManager;

	/// <summary> 
	/// Graphics handler of this application.
	/// </summary>
	std::unique_ptr<AppGfx> gfx;

#ifdef WINDOWS_PLATFORM
	HWND coreWnd;
#endif // WINDOWS_PLATFORM

	public:

	/// <summary> 
	/// Singleton instance of this application.
	/// </summary>
	static App& Instance();

	/// <summary> 
	/// Initalizes application resources.
	/// </summary>
	void Initialize();

	/// <summary> 
	/// Creates app core window and displays it.
	/// </summary>
	void CreateCoreWindow(AppCoreWindowParams&& params);

	/// <summary> 
	/// This is called when application is loading.
	/// </summary>
	void OnLoad();

	/// <summary> 
	/// Runs app message queue.
	/// </summary>
	void RunMsgLoop();

	/// <summary> 
	/// Run main app loop.
	/// </summary>
	void Run();

	/// <summary> 
	/// This is called when application is in suspend mode.
	/// </summary>
	void OnSuspend();

	/// <summary> 
	/// TODO: Temporary, needs to be removed on release.
	/// </summary>
	void TempOnTick();

	/// <summary> 
	/// This is called per frame.
	/// </summary>
	void Tick();
};