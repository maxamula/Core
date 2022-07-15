#include "Debug.h"
#include "Graphics.h"	// Graphics
#include "GameObject.h"	// Scene
// imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// Global declarations
DBG_WINDOW dbgWindow = DBG_WINDOW::DBG_WINDOW_NONE;
GameObject* pDbgObject = NULL;

extern Graphics graphics;
extern Scene* pActiveScene;

// func decl
void SceneDbgWindow();

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
		case DBG_WINDOW::DBG_WINDOW_SCENE:
			SceneDbgWindow();
			break;
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}	
}

// Scene
void SceneDbgWindow()
{
	if (pActiveScene == NULL)
		return;
	ImGui::Begin("Current Scene");
	ImGui::End();
}
