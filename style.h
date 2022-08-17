#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

void CustomStyle() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(0.00f, 0.00f);
	style.FramePadding = ImVec2(0.00f, 0.00f);
	style.CellPadding = ImVec2(0.00f, 0.00f);
	style.ItemSpacing = ImVec2(8.00f, 4.00f);
	style.ItemInnerSpacing = ImVec2(4.00f, 4.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 10.00f;
	style.ScrollbarSize = 10.00f;
	style.GrabMinSize = 10.00f;
	style.WindowBorderSize = 0.00f;
	style.ChildBorderSize = 1.00f;
	style.PopupBorderSize = 0.00f;
	style.FrameBorderSize = 0.00f;
	style.TabBorderSize = 0.00f;
	style.WindowRounding = 0.00f;
	style.ChildRounding = 0.00f;
	style.FrameRounding = 0.00f;
	style.PopupRounding = 0.00f;
	style.ScrollbarRounding = 6.00f;
	style.GrabRounding = 3.00f;
	style.LogSliderDeadzone = 4.00f;
	style.TabRounding = 0.00f;
	style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
	style.WindowMenuButtonPosition = 0;
	style.ColorButtonPosition = 1;
	style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
	style.SelectableTextAlign = ImVec2(0.00f, 0.00f);
	style.DisplaySafeAreaPadding = ImVec2(0.00f, 0.00f);
}

