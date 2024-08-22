#pragma once
#include "Dependencies/Dependencies.h"
#include "Dependencies/Math/Math.hpp"

// Config
#include "config.h"

// ui
#include "gui.h"
#include "SDK/SDK.hpp"

namespace moneyhack
{
     
    inline INT entry()
    {
		if (!Driver->Attach(L"cs2.exe"))
		{
			return 0x01;
		}
		

		gui::CreateHWindow("Moneyhack");
		gui::CreateDevice();
		gui::CreateImGui();
		std::thread win(gui::windowthread);
		win.detach();
		SetWindowPos(gui::window, HWND_TOPMOST, 500, 500, gui::WIDTH, gui::HEIGHT, NULL);
		storage::clientdll = Driver->GetModuleBase(L"client.dll");
		// start thread
		SDK::entities->Run();
		while (gui::isRunning)
		{
			gui::BeginRender();
			gui::Render();
			gui::EndRender();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		// destroy gui
		gui::DestroyImGui();
		gui::DestroyDevice();
		gui::DestroyHWindow();
    }
}