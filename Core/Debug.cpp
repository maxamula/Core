#include "Debug.h"
// imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// Global declarations
DBG_WINDOW dbgWindow = DBG_WINDOW::DBG_WINDOW_NONE;

void DrawDebugWindow()
{
	if (dbgWindow == DBG_WINDOW::DBG_WINDOW_NONE)
		return;
	else
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();

		switch (dbgWindow)
		{
		case DBG_WINDOW::DBG_WINDOW_TEST:
			ImGui::ShowDemoWindow();
			break;
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}	
}
