#include "App.h"

//// DEBUGGING /////////
#include <iostream>
////////////////////////

#include "SnakeGame.h"

App::App()
{
	wndFrameColor = { 1, 1, 1, 1 };
	running = false;
	name = "";
	input = nullptr;
	mapManager = nullptr;
}

App::~App()
{
}

App & App::Instance()
{
	static App app;
	return app;
}

void App::Initialize()
{
	input = std::unique_ptr<AppInput>(new AppInput);
	gfx = std::unique_ptr<AppGfx>(new AppGfx);
	mapManager = std::unique_ptr<MapManager>(new MapManager());
}

void App::TempOnTick()
{
	SnakeGame().Instance().Tick();
}

// TODO: TEMP TESTING
void App::OnLoad()
{
	input->Init();
	gfx->Init();

	SnakeGame().Instance().Initialize();
	SnakeGame().Instance().CreateMaps();
	//mapManager->SetActiveMap(myMap);

	//snakeGame.CreateMaps();
	//snakeGame.SetActiveMap(SnakeGameMap::MainMenu);
}


void App::Tick()
{
	input->Read();
	TempOnTick(); // TODO: !!!!! ORDERING
	mapManager->Update();
	UI::Update();
	gfx->BeginDraw();
	gfx->Clear();
	SnakeGame().Instance().Draw();
	SnakeGame::Instance().DrawTopMost(); // Dammint
	mapManager->Render();
	UI::Render();
	gfx->EndDraw();
	gfx->Present(1, 0);
}


void App::Run()
{
	RunMsgLoop();
}


void App::OnSuspend()
{
	SnakeGame::Instance().SaveScore();
	mapManager.reset();
	input.reset();
}

// Core Application stuff
#pragma region App Core

#if WINDOWS_PLATFORM
long __stdcall CoreWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

void App::CreateCoreWindow(AppCoreWindowParams&& params)
{

#if WINDOWS_PLATFORM

	WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = CreateSolidBrush(RGB(wndFrameColor.r, wndFrameColor.g, wndFrameColor.b));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hInstance = GetModuleHandle(nullptr);
	wcex.lpfnWndProc = CoreWndProc;
	wcex.lpszClassName = "CLS924124822"; // TODO: CHANGE TO GUIDs
	wcex.lpszMenuName = nullptr;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wcex);

	coreWnd = CreateWindow(
		wcex.lpszClassName,
		params.titlebarName.c_str(),
		params.windowStyle,
		params.position.x,
		params.position.y,
		params.resolution.x,
		params.resolution.y,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	ShowWindow(coreWnd, SW_SHOWDEFAULT);
	UpdateWindow(coreWnd);
	SetFocus(coreWnd);
	ShowCursor(params.hardwareCursorEnabled);

#endif

}

void App::RunMsgLoop()
{
	running = true;

	while (running)
	{
#if WINDOWS_PLATFORM

		running = coreWnd != nullptr;

		MSG msg{ 0 };

		running = msg.message != WM_QUIT;

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else Tick();
#endif
	}
}


#if WINDOWS_PLATFORM
	long __stdcall CoreWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			default:
				break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
#endif

#pragma endregion